#include "GameScene.h"
#include "ViciClient.h"
#include "NetworkAsset.h"
#include "Texture.h"
#include <SDL2/SDL.h>
#include <memory>

namespace Scenes {
	Scenes::GameScene::GameScene() {
	}

	Scenes::GameScene::~GameScene() {

	}

	void Scenes::GameScene::update() {
		//TEMP
		if (!_playerTexture)
			_playerTexture.reset(new Networking::NetworkAsset<AssetTypes::Texture>("nik1.png"));
	}

	void Scenes::GameScene::render(SDL_Renderer* renderer) {
		//TEMP
		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = 48;
		src.h = 64;

		SDL_Rect dest{ src };
		if (_playerTexture->getValue())
			SDL_RenderCopy(renderer, _playerTexture->getValue()->getUnderlyingTexture(), &src, &dest);
	}
}