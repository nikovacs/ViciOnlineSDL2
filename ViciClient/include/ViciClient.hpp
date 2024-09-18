#pragma once

#include "header_wrappers/sdl_wrapper.h"
#include <string>
#include <memory>
#include <atomic>
#include <thread>
#include "ViciClient/include/Scenes/Scene.hpp"
#include "ViciClient/include/Scenes/SceneManager.hpp"
#include "ViciClient/include/Networking/UdpClient.hpp"
#include "ViciClient/include/GUI/ViciGuiHost.hpp"

class ViciClient {
public:
	inline static ViciClient* instance{ nullptr };
	
	ViciClient(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	virtual ~ViciClient();

	void start();
	void stop();

	void initialize();

	void handleEvents();
	void update();
	void render();

	bool isRunning() { return _isRunning; }
	SDL_Renderer* getRenderer() { return _renderer; }
	SDL_Window* getWindow() { return _window; }
	
private:
	std::unique_ptr<Scenes::SceneManager> _sceneManager;
	std::atomic_bool _isRunning;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
};