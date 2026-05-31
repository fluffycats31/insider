#include <utils/process-utils.hpp>

namespace process_utils {
	PEPROCESS get_process_object(uint32_t process_id) {
		PEPROCESS process = nullptr;
		if (!NT_SUCCESS(PsLookupProcessByProcessId(ULongToHandle(process_id), &process)))
			return nullptr;

		return process;
	}

	PETHREAD get_thread_object(uint32_t thread_id) {
		PETHREAD thread = nullptr;
		if (!NT_SUCCESS(PsLookupThreadByThreadId(ULongToHandle(thread_id), &thread)))
			return nullptr;

		return thread;
	}

	HANDLE open_process(uint32_t process_id) {
		auto process = get_process_object(process_id);
		if (!process)
			return nullptr;

		HANDLE process_handle = nullptr;
		if (!NT_SUCCESS(ObOpenObjectByPointer(process, OBJ_KERNEL_HANDLE, nullptr,
			PROCESS_ALL_ACCESS,
			*PsProcessType,
			KernelMode,
			&process_handle))) {
			ObfDereferenceObject(process);
			return nullptr;
		}

		ObfDereferenceObject(process);
		return process_handle;
	}

	HANDLE open_thread(uint32_t thread_id) {
		auto thread = get_thread_object(thread_id);
		if (!thread)
			return nullptr;

		HANDLE thread_handle = nullptr;
		if (!NT_SUCCESS(ObOpenObjectByPointer(thread, OBJ_KERNEL_HANDLE, nullptr,
			THREAD_ALL_ACCESS,
			*PsThreadType,
			UserMode,
			&thread_handle))) {
			ObfDereferenceObject(thread);
			return nullptr;
		}

		ObfDereferenceObject(thread);
		return thread_handle;
	}
}