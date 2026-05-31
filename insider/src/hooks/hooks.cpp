#include <hooks/hooks.hpp>

namespace hooks {
	bool initialize() {
		if (!tcpip::initialize()) {
			tcpip::deinitialize();
			return false;
		}

		return true;
	}

	bool deinitialize() {
		tcpip::deinitialize();
		return true;
	}
}