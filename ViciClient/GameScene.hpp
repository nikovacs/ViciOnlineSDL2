#pragma once
#include "Scene.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include "ClientPlayer.hpp"
#include "Level.hpp"
#include "NetworkAsset.hpp"
#include "Camera.hpp"
#include "LevelWatchingManager.hpp"
#include "ClientScriptLoader.hpp"

namespace Scenes {
	class GameScene : public Scene {
	public:
		GameScene(std::string_view sceneName, int x, int y, int w, int h, int dir, std::string_view animation, std::string_view world);
		~GameScene() override;
		void update() override;
		void render(SDL_Renderer* renderer) override;
		void start() override;
		void stop() override;
		Client::Camera& getCamera();
		static GameScene* instance;
	private:
		std::unique_ptr<Entities::ClientPlayer> _clientPlayer{ nullptr };
		std::unique_ptr<Networking::NetworkAsset<Levels::Level>> _world{ nullptr };
		Client::Camera _camera{};
		Networking::LevelWatchingManager _levelWatchingManager{};
		Networking::UdpClient _udpClient;
		std::unique_ptr<std::thread> _networkThread{ nullptr };
		JS::ClientScriptLoader _scriptLoader;
	};
}