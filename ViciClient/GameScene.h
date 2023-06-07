#pragma once
#include "Scene.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace Scenes {
	class GameScene : public Scene {
	public:
		GameScene();
		~GameScene() override;
		void update() override;
		void render() override;
	private:
		SDL_Texture* _playerTexture;
	};
}