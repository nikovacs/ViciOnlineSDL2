#include "GameScene.h"
#include <string>

using namespace std::string_view_literals;

void Scenes::SceneManager::initialize() {
	_scenes = std::map<std::string_view, Scene>();
	
	// TODO add other scenes
	_scenes.emplace("game"sv, GameScene());

	// TODO make the default scene the login scene when ready
	_currentScene = _scenes.at("game"sv);
}

void Scenes::SceneManager::update() {
	_currentScene.get().update();
}

void Scenes::SceneManager::render() {
	_currentScene.get().render();
}

void Scenes::SceneManager::uninitialize() {
	_currentScene.get().uninitialize();
}

void Scenes::SceneManager::setScene(std::string_view name) {
	_currentScene = _scenes.at(name);
}

void Scenes::SceneManager::resetScene(std::string_view name) {
	_scenes.at(name).uninitialize();
	_scenes.at(name).initialize();
}

