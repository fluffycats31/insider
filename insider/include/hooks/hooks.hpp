#ifndef hooks_hpp
#define hooks_hpp

#include <ntifs.h>
#include <ntddk.h>
#include <cstdint>

#include <hooks/tcpip.hpp>

namespace hooks {
	bool initialize();
	bool deinitialize();
}

#endif // !hooks_hpp