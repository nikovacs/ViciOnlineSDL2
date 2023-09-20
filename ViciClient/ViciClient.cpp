#include "ViciClient.hpp"
#include "Scene.hpp"
#include <SDL2/SDL.h>
#include "UdpClient.hpp"
#include "KeyboardInputHandler.hpp"
#include "MouseInputHandler.hpp"
#include <iostream>
#include <thread>

ViciClient::ViciClient(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
	: _isRunning{ false }, _window{ nullptr }, _renderer{ nullptr }, _sceneManager{ std::make_unique<Scenes::SceneManager>() } {
	int flags{ 0 };
	flags |= fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE;
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { return; }
	_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
	_renderer = SDL_CreateRenderer(_window, -1, 0);
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);

	GUI::ViciGuiHost::initialize(_window, _renderer);

	_sceneManager->newLoginScene();
	_sceneManager->setScene("LoginScene");

	instance = this;
}

ViciClient::~ViciClient() {
	// scene manager needs to dispose of rmlContexts before ViciGuiHost deinitializes
	_sceneManager.reset();
	GUI::ViciGuiHost::deinitialize();
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}

void ViciClient::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);
	_sceneManager->handleEvents(&event);

	switch (event.type) {
	case SDL_QUIT:
		stop();
		break;
	case SDL_KEYDOWN:
		Handlers::KeyboardInputHandler::handleKeyDown(event);
		break;
	case SDL_KEYUP:
		Handlers::KeyboardInputHandler::handleKeyUp(event);
		break;
	case SDL_MOUSEBUTTONDOWN:
		Handlers::MouseInputHandler::handleMouseDown(event);
		break;
	case SDL_MOUSEBUTTONUP:
		Handlers::MouseInputHandler::handleMouseUp(event);
		break;
	default:
		break;
	}
}

void ViciClient::initialize() {
}

void ViciClient::update() {
	_sceneManager->update();
}

void ViciClient::render() {
	SDL_RenderClear(_renderer);
	// do rendering here
	_sceneManager->render(_renderer);
	
	SDL_RenderPresent(_renderer);
}

void ViciClient::start() {
	_isRunning = true;
}

void ViciClient::stop() {
	_isRunning = false;
}