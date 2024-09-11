#include <string>
#include <sstream>
#include "ViciClient/include/Animations/FrameDirection.hpp"

Animations::FrameDirection::FrameDirection(std::string& source) {
	std::string data = source.substr(2);

	std::istringstream iss(data);
	while (std::getline(iss, data, ' ')) {
		std::string spriteIndexOrXOrY{};
		std::istringstream directionStream(data);
		int index{ 0 };
		while (std::getline(directionStream, spriteIndexOrXOrY, ',')) {
			switch (index) {
			case 0:
				spriteIndices.push_back(std::stoi(spriteIndexOrXOrY));
				break;
			case 1:
				spriteXs.push_back(std::stoi(spriteIndexOrXOrY));
				break;
			case 2:
				spriteYs.push_back(std::stoi(spriteIndexOrXOrY));
				break;
			default:
				break;
			}
			++index;
		}
	}
}

Animations::FrameDirection::~FrameDirection() {}
