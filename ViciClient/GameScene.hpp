#pragma once
#include "Scene.hpp"
#include "NetworkAsset.hpp"
#include "Texture.hpp"
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