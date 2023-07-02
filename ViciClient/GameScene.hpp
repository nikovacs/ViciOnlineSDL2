#pragma once
#include "Scene.hpp"
#include "EntityTexture.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>

namespace Scenes {
	class GameScene : public Scene {
	public:
		GameScene();
		~GameScene() override;
		void initialize() override;
		void update() override;
		void render(SDL_Renderer* renderer) override;
	private:
		//std::unique_ptr<Networking::NetworkAsset<AssetTypes::Texture>> _playerTexture{ nullptr };
		std::unique_ptr<Entities::EntityTexture> _playerTexture{ nullptr };
	};
}