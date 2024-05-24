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
#include "../ViciEngine/Attributes.hpp"
#include <thread>

namespace Scenes {
	class GameScene : public Scene {
	public:
		GameScene(std::string_view sceneName, std::string_view serverUrl, int serverPort);
		~GameScene() override;
		void update() override;
		void render(SDL_Renderer* renderer) override;
		void loadInitPlayerData(nlohmann::json& json);
		Client::Camera& getCamera();
		JS::ClientScriptLoader& getScriptLoader();
		static GameScene* instance;
	private:
		std::unique_ptr<Entities::ClientPlayer> _clientPlayer{ nullptr };
		std::unique_ptr<Networking::NetworkAsset<Levels::Level>> _world{ nullptr };
		Client::Camera _camera{};
		Networking::LevelWatchingManager _levelWatchingManager{};
		Networking::UdpClient _udpClient;
		std::unique_ptr<std::thread> _networkThread{ nullptr };
		JS::ClientScriptLoader _scriptLoader{};
	};
}