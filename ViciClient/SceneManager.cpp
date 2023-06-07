#include "GameScene.h"
#include "Scene.h"
#include <string>
#include <memory>
#include <SDL2/sdl.h>

using namespace std::string_view_literals;

namespace Scenes {
	SceneManager::SceneManager() {
		_scenes = std::map<std::string_view, std::unique_ptr<Scene>>();

		// TODO add other scenes
		_scenes.emplace("game"sv, std::unique_ptr<Scene>(new GameScene()));

		// TODO make the default scene the login scene when ready
		_currentScene = _scenes.at("game"sv).get();
	}
	
	SceneManager::~SceneManager() {

	}

	void SceneManager::update() {
		_currentScene->update();
	}

	void SceneManager::render(SDL_Renderer* renderer) {
		_currentScene->render(renderer);
	}

	void SceneManager::setScene(std::string_view name) {
		_currentScene = _scenes.at(name).get();
	}
}

