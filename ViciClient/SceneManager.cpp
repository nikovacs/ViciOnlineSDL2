#include "GameScene.h"
#include "Scene.h"
#include <string>
#include <memory>

using namespace std::string_view_literals;

namespace Scenes {
	void SceneManager::initialize() {
		_scenes = std::map<std::string_view, std::unique_ptr<Scene>>();

		// TODO add other scenes
		_scenes.emplace("game"sv, std::unique_ptr<Scene>(new GameScene()));

		// TODO make the default scene the login scene when ready
		_currentScene = _scenes.at("game"sv).get();
	}

	void SceneManager::update() {
		_currentScene->update();
	}

	void SceneManager::render() {
		_currentScene->render();
	}

	void SceneManager::uninitialize() {
		_currentScene->uninitialize();
	}

	void SceneManager::setScene(std::string_view name) {
		_currentScene = _scenes.at(name).get();
	}

	void SceneManager::resetScene(std::string_view name) {
		_scenes.at(name)->uninitialize();
		_scenes.at(name)->initialize();
	}
}

