#ifndef tcpip_hpp
#define tcpip_hpp

#include <ntifs.h>
#include <ntddk.h>
#include <cstdint>

#include <utils/system-utils.hpp>

#define NDIS660 1
#define NDIS_PROTOCOL_DRIVER 1

#include <ndis.h>

#define CONCAT_HIDDEN(x, y) x ## y
#define CONCAT(x, y) CONCAT_HIDDEN(x, y)

#define PAD(size) uint8_t CONCAT(padding_, __LINE__)[size]

namespace interception {
	using NDIS_REFCOUNT_HANDLE__ = unsigned int;

	typedef struct _REFERENCE_EX {
		unsigned __int64 SpinLock;
		unsigned __int16 ReferenceCount;
		unsigned __int8 Closing;
		unsigned __int8 ZeroBased;
		NDIS_REFCOUNT_HANDLE__* RefCountTracker;
	}REFERENCE_EX, * PREFERENCE_EX;

	using prot_send_net_buffer_lists_complete_t = void(__cdecl*)(NDIS_HANDLE, PNET_BUFFER_LIST, uint32_t);
	using receive_net_buffer_lists_t = void(__cdecl*)(NDIS_HANDLE, PNET_BUFFER_LIST, NDIS_PORT_NUMBER,
		uint32_t,
		uint32_t);

	using bind_adapater_handler_ex_t = NDIS_STATUS(__fastcall*)(NDIS_HANDLE protocol_driver_context, NDIS_HANDLE bind_context, PNDIS_BIND_PARAMETERS bind_parameters);
	using unbind_adapater_handler_ex_t = NDIS_STATUS(__fastcall*)(NDIS_HANDLE unbind_context, NDIS_HANDLE protocol_binding_context);

	typedef struct _NDIS_OPEN_BLOCK {
		PAD(0x208);
		prot_send_net_buffer_lists_complete_t ProtSendNetBufferListsComplete;
		PAD(0x10);
		receive_net_buffer_lists_t ReceiveNetBufferLists;
	}NDIS_OPEN_BLOCK, * PNDIS_OPEN_BLOCK;

	union $7C9A2C510B83847395C6471B35D6372E {
		int(__fastcall* PnPEventHandler)(void*, _NET_PNP_EVENT*);
		int(__fastcall* NetPnPEventHandler)(void*, _NET_PNP_EVENT_NOTIFICATION*);
	};

	union $3383BDE40ACFE8DB9524CD1FBC20911C {
		void(__fastcall* StatusHandlerEx)(void*, _NDIS_STATUS_INDICATION*);
		void(__fastcall* StatusHandler)(void*, int, void*, unsigned int);
	};

	union $A722635E5973D4EA05DE2343EF2E6038 {
		void(__fastcall* CoStatusHandlerEx)(void*, void*, _NDIS_STATUS_INDICATION*);
		void(__fastcall* CoStatusHandler)(void*, void*, int, void*, unsigned int);
	};

	union $AB2773F7C4E273F0882611C817D2AA5D {
		void(__fastcall* SendCompleteHandler)(void*, struct _NDIS_PACKET*, int);
		void(__fastcall* WanSendCompleteHandler)(void*, struct _NDIS_WAN_PACKET*, int);
	};

	union $1FA10337BDFCD8A9F52A7369DA14FD16 {
		void(__fastcall* TransferDataCompleteHandler)(void*, struct _NDIS_PACKET*, int, unsigned int);
		void(__fastcall* WanTransferDataCompleteHandler)();
	};

	union $E9E608D0699F71676FEA5C9A233E13E0 {
		int(__fastcall* ReceiveHandler)(void*, void*, void*, unsigned int, void*, unsigned int, unsigned int);
		int(__fastcall* WanReceiveHandler)(void*, unsigned __int8*, unsigned int);
	};

	typedef struct _NDIS_PROTOCOL_BLOCK {
		_NDIS_OBJECT_HEADER Header;
		void* ProtocolDriverContext;
		_NDIS_PROTOCOL_BLOCK* NextProtocol;
		_NDIS_OPEN_BLOCK* OpenQueue;
		_REFERENCE_EX Ref;
		unsigned __int8 MajorNdisVersion;
		unsigned __int8 MinorNdisVersion;
		unsigned __int8 MajorDriverVersion;
		unsigned __int8 MinorDriverVersion;
		unsigned int Reserved;
		unsigned int Flags;
		_UNICODE_STRING Name;
		unsigned __int8 IsIPv4;
		unsigned __int8 IsIPv6;
		unsigned __int8 IsNdisTest6;
		int(__fastcall* BindAdapterHandlerEx)(void*, void*, _NDIS_BIND_PARAMETERS*);
		int(__fastcall* UnbindAdapterHandlerEx)(void*, void*);
		void(__fastcall* OpenAdapterCompleteHandlerEx)(void*, int);
		void(__fastcall* CloseAdapterCompleteHandlerEx)(void*);
		$7C9A2C510B83847395C6471B35D6372E ___u19;
		void(__fastcall* UnloadHandler)();
		void(__fastcall* UninstallHandler)();
		void(__fastcall* RequestCompleteHandler)(void*, struct _NDIS_REQUEST*, int);
		$3383BDE40ACFE8DB9524CD1FBC20911C ___u23;
		void(__fastcall* StatusCompleteHandler)(void*);
		void(__fastcall* ReceiveNetBufferListsHandler)(void*, _NET_BUFFER_LIST*, unsigned int, unsigned int, unsigned int);
		void(__fastcall* SendNetBufferListsCompleteHandler)(void*, _NET_BUFFER_LIST*, unsigned int);
		$A722635E5973D4EA05DE2343EF2E6038 ___u27;
		void(__fastcall* CoAfRegisterNotifyHandler)(void*, CO_ADDRESS_FAMILY*);
		void(__fastcall* CoReceiveNetBufferListsHandler)(void*, void*, _NET_BUFFER_LIST*, unsigned int, unsigned int);
		void(__fastcall* CoSendNetBufferListsCompleteHandler)(void*, _NET_BUFFER_LIST*, unsigned int);
		void(__fastcall* OpenAdapterCompleteHandler)(void*, int, int);
		void(__fastcall* CloseAdapterCompleteHandler)(void*, int);
		$AB2773F7C4E273F0882611C817D2AA5D ___u33;
		$1FA10337BDFCD8A9F52A7369DA14FD16 ___u34;
		void(__fastcall* ResetCompleteHandler)(void*, int);
		$E9E608D0699F71676FEA5C9A233E13E0 ___u36;
		void(__fastcall* ReceiveCompleteHandler)(void*);
		int(__fastcall* ReceivePacketHandler)(void*, struct _NDIS_PACKET*);
		void(__fastcall* BindAdapterHandler)(int*, void*, _UNICODE_STRING*, void*, void*);
		void(__fastcall* UnbindAdapterHandler)(int*, void*, void*);
		void(__fastcall* CoSendCompleteHandler)(int, void*, struct _NDIS_PACKET*);
		unsigned int(__fastcall* CoReceivePacketHandler)(void*, void*, struct _NDIS_PACKET*);
		void(__fastcall* OidRequestCompleteHandler)(void*, _NDIS_OID_REQUEST*, int);
		_WORK_QUEUE_ITEM WorkItem;
		_KMUTANT Mutex;
		void* MutexOwnerThread;
		unsigned int MutexOwnerCount;
		_UNICODE_STRING* BindDeviceName;
		_UNICODE_STRING* RootDeviceName;
		_NDIS_M_DRIVER_BLOCK* AssociatedMiniDriver;
		_NDIS_MINIPORT_BLOCK* BindingAdapter;
		_KEVENT* DeregEvent;
		_NDIS_CO_CLIENT_OPTIONAL_HANDLERS ClientChars;
		_NDIS_CO_CALL_MANAGER_OPTIONAL_HANDLERS CallMgrChars;
		void(__fastcall* DirectOidRequestCompleteHandler)(void*, _NDIS_OID_REQUEST*, int);
		int(__fastcall* AllocateSharedMemoryHandler)(void*, _NDIS_SHARED_MEMORY_PARAMETERS*, void**);
		void(__fastcall* FreeSharedMemoryHandler)(void*, void*);
		void* AllocateSharedMemoryContext;
		_UNICODE_STRING ImageName;
	}NDIS_PROTOCOL_BLOCK, * PNDIS_PROTOCOL_BLOCK;

	NDIS_STATUS bind_adapter_handler_ex_hook(NDIS_HANDLE protocol_driver_context, NDIS_HANDLE bind_context, PNDIS_BIND_PARAMETERS bind_parameters);
	NDIS_STATUS unbind_adapter_handler_ex_hook(NDIS_HANDLE unbind_context, NDIS_HANDLE protocol_binding_context);

	void interception_null_function();

	using on_notify_adapater_bind_t = void(*)();
	using on_notify_adapater_unbind_t = void(*)();

	typedef struct _interception_block {
		// globals
		PNDIS_PROTOCOL_BLOCK tcpip_protocol_block = nullptr;
		// hooks
		prot_send_net_buffer_lists_complete_t prot_send_net_buffer_lists_hook = nullptr;
		receive_net_buffer_lists_t receive_net_buffer_lists_hook = nullptr;
		// original
		bind_adapater_handler_ex_t o_bind_adapater_handler_ex = nullptr;
		unbind_adapater_handler_ex_t o_unbind_adapater_handler_ex = nullptr;
		prot_send_net_buffer_lists_complete_t o_prot_send_net_buffer_lists_hook = nullptr;
		receive_net_buffer_lists_t o_receive_net_buffer_lists_hook = nullptr;
		// callbacks
		on_notify_adapater_bind_t on_notify_adapater_bind = nullptr;
		on_notify_adapater_unbind_t on_notify_adapater_unbind = nullptr;
	}interception_block, *pinterception_block;

	extern interception_block g_interception_block;

	pinterception_block initialize(prot_send_net_buffer_lists_complete_t send_net_buffer_lists_hook, receive_net_buffer_lists_t recieve_net_buffer_lists_hook, on_notify_adapater_bind_t bind_callback,
		on_notify_adapater_unbind_t unbind_callback);
	bool deinitialize();
}

namespace hooks::tcpip {
	extern interception::pinterception_block interception_block;

	void adapater_bind_callback();
	void adapater_unbind_callback();
	void send_net_buffer_lists_hook(NDIS_HANDLE protocol_binding_context, PNET_BUFFER_LIST net_buffer_lists, uint32_t send_complete_flags);
	void recieve_net_buffer_list_hook(NDIS_HANDLE protocol_binding_context, PNET_BUFFER_LIST net_buffer_lists, NDIS_PORT_NUMBER port_number,
		uint32_t number_of_net_buffer_lists,
		uint32_t receive_flags);

	bool initialize();
	bool deinitialize();
}

#endif // !tcpip_hpp