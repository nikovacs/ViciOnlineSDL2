#define SDL_MAIN_HANDLED
#include "header_wrappers/sdl_wrapper.h"
#include <iostream>
#include "ViciClient/include/ViciClient.hpp"
#include "ViciClient/include/TimeManager.hpp"

int main() {
	ViciClient client{ "Vici Online", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, false };
	client.start();
	client.initialize();

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	SDL_SetWindowSize(client.getWindow(), displayMode.w, displayMode.h);
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