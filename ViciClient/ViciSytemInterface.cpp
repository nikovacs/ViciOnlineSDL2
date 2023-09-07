#include "ViciSystemInterface.hpp"

namespace GUI {
	ViciSystemInterface::ViciSystemInterface() : _startTime{ std::chrono::steady_clock::now() } {
		Rml::SetSystemInterface(this);
	}
	
	ViciSystemInterface::~ViciSystemInterface() {
		Rml::Shutdown();
	}

	double ViciSystemInterface::GetElapsedTime() {
		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsedTime = now - _startTime;
		return elapsedTime.count();
	}
}