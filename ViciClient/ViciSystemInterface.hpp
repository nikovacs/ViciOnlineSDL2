#pragma once

#include <RmlUi/Core.h>
#include <chrono>

namespace GUI {
	class ViciSystemInterface : public Rml::SystemInterface {
	public:
		ViciSystemInterface();
		virtual ~ViciSystemInterface();
		double GetElapsedTime() override;
	private:
		std::chrono::time_point<std::chrono::steady_clock> _startTime;
	};
}