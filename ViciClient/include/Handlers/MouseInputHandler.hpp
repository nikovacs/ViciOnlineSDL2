#pragma once

#include <string>
#include <set>
#include "header_wrappers/sdl_wrapper.h"

namespace Handlers {
	class MouseInputHandler {
	public:
		static void handleMouseUp(SDL_Event& e);
		static void handleMouseDown(SDL_Event& e);
		static bool isMouseButtonDown(std::string_view button);
	private:
		static std::set<std::string> _mouseButtonsDown;
		static std::string _getMouseBtnLabel(Uint8 e);
	};
}