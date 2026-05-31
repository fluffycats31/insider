#ifndef memory_utils_hpp
#define memory_utils_hpp

#include <ntifs.h>
#include <ntddk.h>
#include <cstdint>

namespace memory_utils {
	bool read_physical_memory(void* src, void* dst, const size_t size,
		size_t* bytes_transferred);

	bool write_physical_memory(void* src, void* dst, const size_t size,
		size_t* bytes_transferred);

	bool read_physical_memory(void* src, void* dst, const size_t size);

	bool write_physical_memory(void* src, void* dst, const size_t size);

	void* get_physical_for_virtual(void* virtual_address);

	void* get_virtual_for_physical(void* physical_address);
}

#endif // !memory_utils_hpp