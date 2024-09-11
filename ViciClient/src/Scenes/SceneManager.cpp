#include <string>
#include <string_view>
#include <memory>
#include <SDL2/sdl.h>
#include <mutex>
#include <nlohmann/json.hpp>
#include "ViciClient/include/ViciClient.hpp"
#include "ViciClient/include/Scenes/SceneManager.hpp"
#include "ViciClient/include/Scenes/GameScene.hpp"
#include "ViciClient/include/Scenes/LoginScene.hpp"
#include "ViciClient/include/Scenes/Scene.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/ViciAsyncDocumentLoader.hpp"

using namespace std::string_view_literals;

namespace Scenes {
	SceneManager* SceneManager::instance = nullptr;

	SceneManager::SceneManager() {
		instance = this;
	}
	
	SceneManager::~SceneManager() {

	}

	void SceneManager::newGameScene() {
		auto newGameScene = std::make_unique<GameScene>("GameScene"sv, "localhost", 8424);
		
		std::lock_guard<std::recursive_mutex> lock(_sceneMutex);
		_currentScene = nullptr;
		if (_scenes.contains("Game"sv)) {
			_scenes.erase("Game"sv);
		}
		_scenes.emplace("Game"sv, std::move(newGameScene)); 
	}

	Scene& SceneManager::getScene(std::string_view name) {
		std::lock_guard<std::recursive_mutex> lock(_sceneMutex);
		return *_scenes.at(name);
	}

	void SceneManager::newLoginScene(std::function<void()> onLoginCallback) {
		auto newLoginScene = std::make_unique<LoginScene>("LoginScene"sv, onLoginCallback);
		std::lock_guard<std::recursive_mutex> lock(_sceneMutex);
		_currentScene = nullptr;
		if (_scenes.contains("LoginScene"sv)) {
			_scenes.erase("LoginScene"sv);
		}
		_scenes.emplace("LoginScene"sv, std::move(newLoginScene));
	}

	void SceneManager::update() {
		std::lock_guard<std::recursive_mutex> lock(_sceneMutex);
		if (_currentScene)
			_currentScene->update();
		JS::ViciAsyncDocumentLoader::attemptResolve();
	}

	void SceneManager::render(SDL_Renderer* renderer) {
		std::lock_guard<std::recursive_mutex> lock(_sceneMutex);
		if (_currentScene)
			_currentScene->render(renderer);
	}

	void SceneManager::handleEvents(SDL_Event* event) {
		std::lock_guard<std::recursive_mutex> lock(_sceneMutex);
		if (_currentScene)
			_currentScene->handleEvents(*event);
	}

	void SceneManager::setScene(std::string_view name) {
		std::lock_guard<std::recursive_mutex> lock(_sceneMutex);
		_currentScene = _scenes.at(name).get();
	}
}

