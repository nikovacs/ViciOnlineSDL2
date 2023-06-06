#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "ViciClient.h"

int main() {
	ViciClient client = ViciClient();
	client.initialize("Vici Online", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	while (client.isRunning()) {
		client.handleEvents();
		client.update();
		client.render();
	}

	client.unitialize();

	return 0;
}