#include "GameScene.hpp"
#include "ViciClient.hpp"
#include "NetworkAsset.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include "Animation.hpp"
#include "SingleLevel.hpp"

namespace Scenes {
	Scenes::GameScene::GameScene() {
	}

	Scenes::GameScene::~GameScene() {

	}

	void Scenes::GameScene::initialize() {
		_clientPlayer = std::make_unique<Entities::ClientPlayer>("player_idle.vani", 0, 0, 3);
		_level = std::make_unique<Networking::NetworkAsset<Levels::Level>>("newlevel.vlvl");
	}

	void Scenes::GameScene::update() {
		if (_clientPlayer)
			_clientPlayer->update();
		if (_level->getValue())
			_level->getValue()->update();
	}

	void Scenes::GameScene::render(SDL_Renderer* renderer) {
		if (_clientPlayer)
			_clientPlayer->render(renderer);
		if (_level->getValue())
			_level->getValue()->render(renderer);
	}
}