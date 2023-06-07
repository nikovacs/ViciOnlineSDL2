#include "GameScene.h"
#include "TextureManager.h"
#include "ViciClient.h"
#include <SDL2/SDL.h>

namespace Scenes {
	Scenes::GameScene::GameScene() {
	}

	Scenes::GameScene::~GameScene() {
		if (_playerTexture)
			SDL_DestroyTexture(_playerTexture);
	}

	void Scenes::GameScene::update() {
		//TEMP
		if (!_playerTexture)
			_playerTexture = Textures::loadTexture("C:/Users/kovac/Desktop/images/nik1.png");
	}

	void Scenes::GameScene::render(SDL_Renderer* renderer) {
		//TEMP
		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = 48;
		src.h = 64;

		SDL_Rect dest{ src };
		SDL_RenderCopy(renderer, _playerTexture, &src, &dest);
	}
}