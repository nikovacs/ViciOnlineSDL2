#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <memory>
#include "Scene.hpp"
#include "UdpClient.hpp"
#include <atomic>
#include <thread>

class ViciClient {
public:
	inline static ViciClient* instance{ nullptr };
	
	ViciClient(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	virtual ~ViciClient();

	void start();
	void stop();

	void handleEvents();
	void update();
	void render();

	bool isRunning() { return _isRunning; }
	SDL_Renderer* getRenderer() { return _renderer; }
	SDL_Window* getWdinow() { return _window; }
	
private:
	std::unique_ptr<std::thread> _networkThread;
	std::unique_ptr<Networking::UdpClient> _udpClient;
	std::unique_ptr<Scenes::SceneManager> _sceneManager;
	std::atomic_bool _isRunning;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
};