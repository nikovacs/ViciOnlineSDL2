#pragma once
#include "Scene.h"
#include "NetworkAsset.h"
#include "Texture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>

namespace Scenes {
	class GameScene : public Scene {
	public:
		GameScene();
		~GameScene() override;
		void update() override;
		void render(SDL_Renderer* renderer) override;
	private:
		std::unique_ptr<Networking::NetworkAsset<AssetTypes::Texture>> _playerTexture{ nullptr };
	};
}