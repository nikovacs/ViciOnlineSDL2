#pragma once
#include "Scene.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include "ClientPlayer.hpp"
#include "Level.hpp"
#include "NetworkAsset.hpp"

namespace Scenes {
	class GameScene : public Scene {
	public:
		GameScene();
		~GameScene() override;
		void initialize() override;
		void update() override;
		void render(SDL_Renderer* renderer) override;
	private:
		//std::unique_ptr<Networking::NetworkAsset<Animations::Animation>> _playerAnimation{ nullptr };
		std::unique_ptr<Entities::ClientPlayer> _clientPlayer{ nullptr };
		std::unique_ptr<Networking::NetworkAsset<Levels::Level>> _level{ nullptr };
	};
}