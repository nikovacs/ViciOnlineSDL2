#include "GameScene.hpp"
#include "ViciClient.hpp"
#include "NetworkAsset.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include "Animation.hpp"

namespace Scenes {
	Scenes::GameScene::GameScene() {
	}

	Scenes::GameScene::~GameScene() {

	}

	void Scenes::GameScene::initialize() {
		_clientPlayer = std::make_unique<Entities::ClientPlayer>("player_idle.vani", 0, 0, 3);
	}

	void Scenes::GameScene::update() {
		if (_clientPlayer)
			_clientPlayer->update();
	}

	void Scenes::GameScene::render(SDL_Renderer* renderer) {
		if (_clientPlayer)
			_clientPlayer->render(renderer);
	}
}