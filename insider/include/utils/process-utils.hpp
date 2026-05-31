#ifndef process_utils_hpp
#define process_utils_hpp

#include <ntifs.h>
#include <ntddk.h>
#include <cstdint>

namespace process_utils {
	PEPROCESS get_process_object(uint32_t process_id);
	PETHREAD get_thread_object(uint32_t thread_id);

	HANDLE open_process(uint32_t process_id);
	HANDLE open_thread(uint32_t thread_id);
}

#endif // !process_utils_hpp