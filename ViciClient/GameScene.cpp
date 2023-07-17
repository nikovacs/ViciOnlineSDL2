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
		_playerAnimation = std::make_unique<Networking::NetworkAsset<Animations::Animation>>("player_walk.vani");
	}

	void Scenes::GameScene::update() {
		if (_playerAnimation->getValue()) {
			_playerAnimation->getValue()->update();
		}

	}

	void Scenes::GameScene::render(SDL_Renderer* renderer) {
		if (_playerAnimation->getValue()) {
			_playerAnimation->getValue()->render(renderer, 0);
		}
	}
}