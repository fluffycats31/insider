#include <hooks/tcpip.hpp>

namespace interception {
	interception_block g_interception_block = {};

	NDIS_STATUS bind_adapter_handler_ex_hook(NDIS_HANDLE protocol_driver_context, NDIS_HANDLE bind_context, PNDIS_BIND_PARAMETERS bind_parameters) {
		if (g_interception_block.o_bind_adapater_handler_ex) {
			const auto status = g_interception_block.o_bind_adapater_handler_ex(protocol_driver_context, bind_context, bind_parameters);
			if (g_interception_block.on_notify_adapater_bind)
				g_interception_block.on_notify_adapater_bind();

			return status;
		} else
			return STATUS_UNSUCCESSFUL;
	}

	NDIS_STATUS unbind_adapter_handler_ex_hook(NDIS_HANDLE unbind_context, NDIS_HANDLE protocol_binding_context) {
		if (g_interception_block.o_unbind_adapater_handler_ex) {
			const auto status = g_interception_block.o_unbind_adapater_handler_ex(unbind_context, protocol_binding_context);
			if (g_interception_block.on_notify_adapater_unbind)
				g_interception_block.on_notify_adapater_unbind();

			return status;
		} else
			return STATUS_UNSUCCESSFUL;
	}

	void interception_null_function() {}

	pinterception_block initialize(prot_send_net_buffer_lists_complete_t send_net_buffer_lists_hook, receive_net_buffer_lists_t recieve_net_buffer_lists_hook, on_notify_adapater_bind_t bind_callback,
		on_notify_adapater_unbind_t unbind_callback) {
		if (!send_net_buffer_lists_hook || !recieve_net_buffer_lists_hook || !bind_callback
			|| !unbind_callback)
			return nullptr;

		g_interception_block.prot_send_net_buffer_lists_hook = send_net_buffer_lists_hook;
		g_interception_block.receive_net_buffer_lists_hook = recieve_net_buffer_lists_hook;
		g_interception_block.on_notify_adapater_bind = bind_callback;
		g_interception_block.on_notify_adapater_unbind = unbind_callback;

		const auto get_tcpip_protocol_block = [&]() -> PNDIS_PROTOCOL_BLOCK {
			NDIS_PROTOCOL_DRIVER_CHARACTERISTICS protocol_characteristics = {};
			PNDIS_OBJECT_HEADER ndis_object_header = &protocol_characteristics.Header;
			ndis_object_header->Type = NDIS_OBJECT_TYPE_PROTOCOL_DRIVER_CHARACTERISTICS;
			ndis_object_header->Revision = NDIS_PROTOCOL_DRIVER_CHARACTERISTICS_REVISION_2;
			ndis_object_header->Size = NDIS_SIZEOF_PROTOCOL_DRIVER_CHARACTERISTICS_REVISION_2;

			protocol_characteristics.MajorNdisVersion = 6;
			protocol_characteristics.MinorNdisVersion = 60;
			protocol_characteristics.MajorDriverVersion = 1;
			protocol_characteristics.MinorDriverVersion = 0;

			protocol_characteristics.Name = RTL_CONSTANT_STRING(L"interception");

			protocol_characteristics.BindAdapterHandlerEx = (BIND_HANDLER_EX)interception_null_function;
			protocol_characteristics.UnbindAdapterHandlerEx = (UNBIND_HANDLER_EX)interception_null_function;
			protocol_characteristics.OpenAdapterCompleteHandlerEx = (OPEN_ADAPTER_COMPLETE_HANDLER_EX)interception_null_function;
			protocol_characteristics.CloseAdapterCompleteHandlerEx = (CLOSE_ADAPTER_COMPLETE_HANDLER_EX)interception_null_function;
			protocol_characteristics.NetPnPEventHandler = (NET_PNP_EVENT_HANDLER)interception_null_function;
			protocol_characteristics.SendNetBufferListsCompleteHandler = (SEND_NET_BUFFER_LISTS_COMPLETE_HANDLER)interception_null_function;
			protocol_characteristics.ReceiveNetBufferListsHandler = (RECEIVE_NET_BUFFER_LISTS_HANDLER)interception_null_function;
			protocol_characteristics.OidRequestCompleteHandler = (OID_REQUEST_COMPLETE_HANDLER)interception_null_function;

			NDIS_HANDLE ndis_protocol_handle = nullptr;
			if (!NT_SUCCESS(NdisRegisterProtocolDriver(nullptr, &protocol_characteristics, &ndis_protocol_handle)))
				return nullptr;

			const UNICODE_STRING tcpip_string = RTL_CONSTANT_STRING(L"TCPIP");
			auto* current_protocol_block = (PNDIS_PROTOCOL_BLOCK)ndis_protocol_handle;
			PNDIS_PROTOCOL_BLOCK protocol_block = nullptr;
			do {
				if (!RtlCompareUnicodeString(&current_protocol_block->Name, &tcpip_string, true)) {
					protocol_block = current_protocol_block;
					break;
				}
				current_protocol_block = current_protocol_block->NextProtocol;
			} while (current_protocol_block);

			NdisDeregisterProtocolDriver(ndis_protocol_handle);
			if (!protocol_block)
				return nullptr;

			return protocol_block;
		};

		const auto get_tcpip_protocol_block_signature = [&]() -> PNDIS_PROTOCOL_BLOCK {
			const auto* rip = system_utils::pattern_scan(L"ndis.sys", "\x48\x8B\x1D\x00\x00\x00\x00\xEB\x00\x48\x8B\x5B", "xxx????x?xxx");
			if (!rip)
				return nullptr;

			const auto rel32 = *(int32_t*)(rip + 0x3);
			
			const UNICODE_STRING tcpip_string = RTL_CONSTANT_STRING(L"TCPIP");
			auto* current_protocol_block = *(PNDIS_PROTOCOL_BLOCK*)(rip + rel32 + 0x7);
			PNDIS_PROTOCOL_BLOCK protocol_block = nullptr;
			do {
				if (!RtlCompareUnicodeString(&current_protocol_block->Name, &tcpip_string, true)) {
					protocol_block = current_protocol_block;
					break;
				}
				current_protocol_block = current_protocol_block->NextProtocol;
			} while (current_protocol_block);

			if (!protocol_block)
				return nullptr;

			return protocol_block;
		};

		g_interception_block.tcpip_protocol_block = get_tcpip_protocol_block_signature();
		if (!g_interception_block.tcpip_protocol_block)
			return nullptr;

		// maybe use _InterlockedExchangePointer later
		g_interception_block.o_bind_adapater_handler_ex = g_interception_block.tcpip_protocol_block->BindAdapterHandlerEx;
		g_interception_block.o_unbind_adapater_handler_ex = g_interception_block.tcpip_protocol_block->UnbindAdapterHandlerEx;

		g_interception_block.tcpip_protocol_block->BindAdapterHandlerEx = bind_adapter_handler_ex_hook;
		g_interception_block.tcpip_protocol_block->UnbindAdapterHandlerEx = unbind_adapter_handler_ex_hook;

		auto* const open_queue = g_interception_block.tcpip_protocol_block->OpenQueue;
		if (open_queue) {
			g_interception_block.o_prot_send_net_buffer_lists_hook = open_queue->ProtSendNetBufferListsComplete;
			g_interception_block.o_receive_net_buffer_lists_hook = open_queue->ReceiveNetBufferLists;

			open_queue->ReceiveNetBufferLists = g_interception_block.receive_net_buffer_lists_hook;
			open_queue->ProtSendNetBufferListsComplete = g_interception_block.prot_send_net_buffer_lists_hook;
		}

		return &g_interception_block;
	}

	bool deinitialize() {
		if (!g_interception_block.tcpip_protocol_block)
			return true;

		g_interception_block.tcpip_protocol_block->BindAdapterHandlerEx = g_interception_block.o_bind_adapater_handler_ex;
		g_interception_block.tcpip_protocol_block->UnbindAdapterHandlerEx = g_interception_block.o_unbind_adapater_handler_ex;

		auto* const open_queue = g_interception_block.tcpip_protocol_block->OpenQueue;
		if (open_queue) {
			open_queue->ReceiveNetBufferLists = g_interception_block.o_receive_net_buffer_lists_hook;
			open_queue->ProtSendNetBufferListsComplete = g_interception_block.o_prot_send_net_buffer_lists_hook;
		}

		g_interception_block = {};
		return true;
	}
}

namespace hooks::tcpip {
	interception::pinterception_block interception_block = nullptr;

	void adapater_bind_callback() {
		const auto* tcpip_protocol_block = interception_block->tcpip_protocol_block;
		if (!tcpip_protocol_block->OpenQueue)
			return;

		auto* const open_queue = tcpip_protocol_block->OpenQueue;
		if (!interception_block->o_prot_send_net_buffer_lists_hook || !interception_block->o_receive_net_buffer_lists_hook) {
			interception_block->o_prot_send_net_buffer_lists_hook = open_queue->ProtSendNetBufferListsComplete;
			interception_block->o_receive_net_buffer_lists_hook = open_queue->ReceiveNetBufferLists;
		}

		open_queue->ReceiveNetBufferLists = recieve_net_buffer_list_hook;
		open_queue->ProtSendNetBufferListsComplete = send_net_buffer_lists_hook;
	}

	void adapater_unbind_callback() {}

	void send_net_buffer_lists_hook(NDIS_HANDLE protocol_binding_context, PNET_BUFFER_LIST net_buffer_lists, uint32_t send_complete_flags) {
		DbgPrint("%s\n", __FUNCTION__);
		if (interception_block->o_prot_send_net_buffer_lists_hook)
			interception_block->o_prot_send_net_buffer_lists_hook(protocol_binding_context, net_buffer_lists, send_complete_flags);
	}

	void recieve_net_buffer_list_hook(NDIS_HANDLE protocol_binding_context, PNET_BUFFER_LIST net_buffer_lists, NDIS_PORT_NUMBER port_number,
		uint32_t number_of_net_buffer_lists,
		uint32_t receive_flags) {
		DbgPrint("%s\n", __FUNCTION__);
		if (interception_block->o_receive_net_buffer_lists_hook)
			interception_block->o_receive_net_buffer_lists_hook(protocol_binding_context, net_buffer_lists, port_number,
				number_of_net_buffer_lists,
				receive_flags);
	}

	bool initialize() {
		interception_block = interception::initialize(send_net_buffer_lists_hook, recieve_net_buffer_list_hook, adapater_bind_callback,
			adapater_unbind_callback);

		return (interception_block);
	}

	bool deinitialize() {
		return interception::deinitialize();
	}
}