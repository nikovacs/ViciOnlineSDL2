#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <memory>
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "UdpClient.hpp"
#include <atomic>
#include <thread>
#include "ViciGuiHost.hpp"

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
	std::string getUserName() { return _userName; }
	
private:
	std::unique_ptr<Scenes::SceneManager> _sceneManager;
	std::atomic_bool _isRunning;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	std::string _userName{};
};