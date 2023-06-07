#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "ViciClient.h"
#include "TimeManager.h"
#include <iostream>

int main() {
	constexpr int FPS{ 60 };
	constexpr int frameDelay{ 1000 / FPS };
	Uint32 frameStart{ 0 };
	Uint32 frameEnd{ 0 };
	int frameTime{ 0 };
	TimeManager::setDelta(0);
	
	
	ViciClient client = ViciClient("Vici Online", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
	client.instance = &client;

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