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
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { return; }
	_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
	_renderer = SDL_CreateRenderer(_window, -1, 0);
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

	_udpClient = std::make_unique<Networking::UdpClient>("localhost", 8424);

	instance = this;
}

ViciClient::~ViciClient() {
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}

void ViciClient::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);
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
	_networkThread = std::make_unique<std::thread>(&Networking::UdpClient::start, _udpClient.get());
}

void ViciClient::stop() {
	_isRunning = false;
	_udpClient->stop();
	_networkThread->join();
}