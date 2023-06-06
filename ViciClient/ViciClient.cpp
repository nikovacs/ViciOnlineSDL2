#include "ViciClient.h"
#include "Scene.h"

ViciClient::ViciClient() : _isRunning{ false }, _window{ nullptr }, _renderer{ nullptr } {}

ViciClient::~ViciClient() {

}

void ViciClient::initialize(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int flags{ 0 };
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { return; }
	_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
	_renderer = SDL_CreateRenderer(_window, -1, 0);
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	_isRunning = true;

	Scenes::SceneManager::initialize();
	
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
	Scenes::SceneManager::update();
}

void ViciClient::render() {
	SDL_RenderClear(_renderer);
	// do rendering here
	Scenes::SceneManager::render();
	
	SDL_RenderPresent(_renderer);
}

void ViciClient::unitialize() {
	Scenes::SceneManager::uninitialize();
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}
