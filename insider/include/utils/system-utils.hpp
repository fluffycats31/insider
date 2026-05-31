#ifndef system_utils_hpp
#define system_utils_hpp

#include <ntifs.h>
#include <ntddk.h>
#include <cstdint>

#include <types/ketypes.hpp>

extern "C" {
	extern PLIST_ENTRY PsLoadedModuleList;
	extern PERESOURCE PsLoadedModuleResource;
};

namespace system_utils {
	PKLDR_DATA_TABLE_ENTRY get_system_module_entry(const wchar_t* module_name);
	uint8_t* pattern_scan(const wchar_t* module_name, const char* pattern, const char* mask);
}

#endif // !system_utils_hpp