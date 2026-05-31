#include <utils/system-utils.hpp>

namespace system_utils {
	PKLDR_DATA_TABLE_ENTRY get_system_module_entry(const wchar_t* module_name) {
		if (!module_name)
			return nullptr;

		UNICODE_STRING module_string = {};
		RtlInitUnicodeString(&module_string, module_name);

		KeEnterCriticalRegion();
		ExAcquireResourceSharedLite(PsLoadedModuleResource, true);

		PKLDR_DATA_TABLE_ENTRY data_table_entry = nullptr;
		auto* list_entry = PsLoadedModuleList->Flink;
		for (; list_entry != PsLoadedModuleList; list_entry = list_entry->Flink) {
			auto* current_date_table_entry = CONTAINING_RECORD(list_entry, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
			if (RtlEqualUnicodeString(&module_string, &current_date_table_entry->BaseDllName, true)) {
				data_table_entry = current_date_table_entry;
				break;
			}
		}

		ExReleaseResourceLite(PsLoadedModuleResource);
		KeLeaveCriticalRegion();
		return data_table_entry;
	}

	uint8_t* pattern_scan(const wchar_t* module_name, const char* pattern, const char* mask) {
		if (!module_name || !pattern || !mask)
			return nullptr;

		const auto* system_module_entry = get_system_module_entry(module_name);
		if (!system_module_entry)
			return nullptr;

		auto* start = (uint8_t*)system_module_entry->DllBase;
		auto* end = start + system_module_entry->SizeOfImage;

		const auto check_mask = [](const char* base, const char* pattern, const char* mask) -> bool {
			for (; *mask; ++base, ++pattern, ++mask)
				if (*mask == 'x' && *base != *pattern)
					return false;

			return true;
		};

		for (auto* address = start; address < end; address++) {
			if (!MmIsAddressValid(address))
				break;

			if (!check_mask((const char*)address, pattern, mask))
				continue;

			return address;
		}
		
		return nullptr;
	}
}