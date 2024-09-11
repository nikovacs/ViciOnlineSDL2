#pragma once

#include <string>
#include <vector>
#include "ViciClient/include/Animations/FrameDirection.hpp"

namespace Animations {
	class Frame {
	public:
		Frame();
		virtual ~Frame();
		void pushFrameDirection(std::string& directionData);
		void setDuration(double seconds);
		double getDuration();
		std::vector<int>& getSpriteIndicies(int dir);
		std::vector<int>& getSpriteXs(int dir);
		std::vector<int>& getSpriteYs(int dir);
	private:
		std::vector<FrameDirection> _frameDirections{};
		double _durationSeconds{ };
	};
}