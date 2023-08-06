#include "GameScene.hpp"
#include "Scene.hpp"
#include <string>
#include <string_view>
#include <memory>
#include <SDL2/sdl.h>
#include <mutex>

#include <iostream>

using namespace std::string_view_literals;

namespace Scenes {
	SceneManager* SceneManager::instance = nullptr;

	SceneManager::SceneManager() {
		std::cout << "SceneManager constructor called" << std::endl;
		instance = this;
	}
	
	SceneManager::~SceneManager() {

	}

	void SceneManager::newGameScene(ENetEvent& event) {
		auto jsonString = std::string(reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength);
		nlohmann::json json{ nlohmann::json::parse(jsonString) };
		int x = json["x"];
		int y = json["y"];
		int w = json["w"];
		int h = json["h"];
		int dir = json["dir"];
		std::string_view animation = json["animation"];
		std::string_view level = json["level"];

		auto newGameScene = std::make_unique<GameScene>(x, y, w, h, dir, animation, level);
		
		std::lock_guard<std::mutex> lock(_sceneMutex);
		_currentScene = nullptr;
		if (_scenes.contains("Game"sv)) {
			_scenes.erase("Game"sv);
		}
		_scenes.emplace("Game"sv, std::move(newGameScene));
	}

	void SceneManager::update() {
		std::lock_guard<std::mutex> lock(_sceneMutex);
		if (_currentScene)
			_currentScene->update();
	}

	void SceneManager::render(SDL_Renderer* renderer) {
		std::lock_guard<std::mutex> lock(_sceneMutex);
		if (_currentScene)
			_currentScene->render(renderer);
	}

	void SceneManager::setScene(std::string_view name) {
		std::lock_guard<std::mutex> lock(_sceneMutex);
		_currentScene = _scenes.at(name).get();
	}
}

