#include "MouseInputHandler.h"
#include <set>
#include <string>

std::set<std::string> Handlers::MouseInputHandler::_mouseButtonsDown = std::set<std::string>();

void Handlers::MouseInputHandler::handleMouseDown(SDL_Event& e) {
	_mouseButtonsDown.insert(_getMouseBtnLabel(e.button.button));
}

void Handlers::MouseInputHandler::handleMouseUp(SDL_Event& e) {
	_mouseButtonsDown.erase(_getMouseBtnLabel(e.button.button));
}

std::string Handlers::MouseInputHandler::_getMouseBtnLabel(Uint8 e) {
	switch (e) {
	case SDL_BUTTON_LEFT:
		return "Left";
	case SDL_BUTTON_MIDDLE:
		return "Middle";
	case SDL_BUTTON_RIGHT:
		return "Right";
	case SDL_BUTTON_X1:
		return "X1";
	case SDL_BUTTON_X2:
		return "X2";
	default:
		return "Unknown";
	}
}