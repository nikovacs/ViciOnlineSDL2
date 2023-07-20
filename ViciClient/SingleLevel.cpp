#include "SingleLevel.hpp"
#include <SDL2/SDL.h>
#include <string_view>
#include <string>
#include <istream>

namespace Levels {
	SingleLevel::SingleLevel(std::string_view name, std::string_view source) : _name{name} {
		std::istringstream iss{ source.data() };
		std::string line;
		int lineIndex{ 0 };
		while (std::getline(iss, line)) {
			if (!lineIndex) {

			}
		}
	}

	SingleLevel::~SingleLevel() {

	}

	void SingleLevel::update() {

	}

	void SingleLevel::render(SDL_Renderer* renderer) {

	}
}