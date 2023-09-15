#include "GameScene.hpp"
#include "Scene.hpp"
#include <string>
#include <string_view>
#include <memory>
#include <SDL2/sdl.h>
#include <mutex>
#include <nlohmann/json.hpp>
#include "LoginScene.hpp"

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

	void SceneManager::newGameScene(nlohmann::json& json) {
		int x = json["x"];
		int y = json["y"];
		int w = json["w"];
		int h = json["h"];
		int dir = json["dir"];
		std::string_view animation = json["animation"];
		std::string_view world = json["world"];

		auto newGameScene = std::make_unique<GameScene>("GameScene"sv, x, y, w, h, dir, animation, world);
		newGameScene->getCamera().setScale(json["cameraZoom"]);
		
		std::lock_guard<std::mutex> lock(_sceneMutex);
		_currentScene = nullptr;
		if (_scenes.contains("Game"sv)) {
			_scenes.erase("Game"sv);
		}
		_scenes.emplace("Game"sv, std::move(newGameScene));
	}

	void SceneManager::newLoginScene() {
		auto newLoginScene = std::make_unique<LoginScene>("LoginScene"sv);
		std::lock_guard<std::mutex> lock(_sceneMutex);
		_currentScene = nullptr;
		if (_scenes.contains("LoginScene"sv)) {
			_scenes.erase("LoginScene"sv);
		}
		_scenes.emplace("LoginScene"sv, std::move(newLoginScene));
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

	void SceneManager::handleEvents(SDL_Event* event) {
		std::lock_guard<std::mutex> lock(_sceneMutex);
		if (_currentScene)
			_currentScene->handleEvents(*event);
	}

	void SceneManager::setScene(std::string_view name) {
		std::lock_guard<std::mutex> lock(_sceneMutex);
		_currentScene = _scenes.at(name).get();
	}

	void SceneManager::start() {
		for (auto& scene : _scenes) {
			scene.second->start();
		}
	}
	
	void SceneManager::stop() {
		for (auto& scene : _scenes) {
			scene.second->stop();
		}
	}
}

