#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "Scene.h"

class ViciClient {
public:
	ViciClient(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	virtual ~ViciClient();

	void handleEvents();
	void update();
	void render();

	bool isRunning() { return _isRunning; }

	Scenes::SceneManager* getSceneManager();
	
private:
	Scenes::SceneManager* _sceneManager;
	bool _isRunning;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
};