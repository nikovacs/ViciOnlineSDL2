#include "KeyboardInputHandler.h"
#include <string>

std::set<std::string> Handlers::KeyboardInputHandler::_keysDown = std::set<std::string>();

void Handlers::KeyboardInputHandler::handleKeyDown(SDL_Event& e) {
	_keysDown.insert(SDL_GetKeyName(e.key.keysym.sym));
}

void Handlers::KeyboardInputHandler::handleKeyUp(SDL_Event& e) {
	_keysDown.erase(SDL_GetKeyName(e.key.keysym.sym));
}
