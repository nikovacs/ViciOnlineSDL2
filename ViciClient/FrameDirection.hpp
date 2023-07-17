#pragma once

#include <string>
#include <vector>

namespace Animations {
	class FrameDirection {
	public:
		FrameDirection(std::string& source);
		virtual ~FrameDirection();
		std::vector<int> spriteIndices{};
		std::vector<int> spriteXs{};
		std::vector<int> spriteYs{};
	};
}