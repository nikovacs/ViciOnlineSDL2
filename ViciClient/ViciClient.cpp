#include "ViciClient.h"
#include "Scene.h"
#include <SDL2/SDL.h>

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
	_isRunning = true;
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
		_isRunning = false;
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