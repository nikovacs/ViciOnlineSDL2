#include "GameScene.hpp"
#include "ViciClient.hpp"
#include "NetworkAsset.hpp"
#include "Texture.hpp"
#include <SDL2/SDL.h>
#include <memory>

namespace Scenes {
	Scenes::GameScene::GameScene() {
	}

	Scenes::GameScene::~GameScene() {

	}

	void Scenes::GameScene::initialize() {
		_playerTexture = std::make_unique<Entities::EntityTexture>("nik1.png", 0, 0, 48, 64);
	}

	void Scenes::GameScene::update() {

	}

	void Scenes::GameScene::render(SDL_Renderer* renderer) {
		if (_playerTexture) {
			_playerTexture->render(renderer);
		}
	}
}