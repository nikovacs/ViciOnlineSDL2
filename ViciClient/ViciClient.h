#pragma once

#include <SDL2/SDL.h>
#include <string>

class ViciClient {
public:
	ViciClient();
	virtual ~ViciClient();

	void initialize(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void unitialize();

	bool isRunning() { return _isRunning; }
	
private:
	bool _isRunning;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
};