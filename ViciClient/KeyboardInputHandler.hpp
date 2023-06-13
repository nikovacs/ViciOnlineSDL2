#pragma once

#include <SDL2/SDL.h>
#include <set>
#include <string>

namespace Handlers {
	class KeyboardInputHandler {
	public:
		static void handleKeyDown(SDL_Event& e);
		static void handleKeyUp(SDL_Event& e);
		static bool isKeyDown(std::string_view key);
	private:
		static std::set<std::string> _keysDown;
	};
}