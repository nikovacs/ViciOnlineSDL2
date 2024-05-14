#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "ViciClient.hpp"
#include "TimeManager.hpp"
#include <iostream>

int main() {
	ViciClient client{ "Vici Online", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false };
	client.start();
	client.initialize();

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	std::cout << "Display refresh rate: " << displayMode.refresh_rate << std::endl;

	int FPS{ displayMode.refresh_rate };
	int frameDelay{ 1000 / FPS };
	Uint32 frameStart{ 0 };
	int frameTime{ 0 };
	TimeManager::setDelta(0);
	
	while (client.isRunning()) {
		Uint32 previousFrameStart{ frameStart };
		frameStart = SDL_GetTicks();
		TimeManager::setDelta(frameStart - previousFrameStart);
		
		client.handleEvents();
		client.update();
		client.render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	return 0;
}