#pragma once

#include "header_wrappers/sdl_wrapper.h"
#include <memory>
#include <thread>
#include "ViciCore/include/Attributes/Attributes.hpp"
#include "ViciClient/include/Camera.hpp"
#include "ViciClient/include/Scenes/Scene.hpp"
#include "ViciClient/include/Levels/Level.hpp"
#include "ViciClient/include/Networking/NetworkAsset.hpp"
#include "ViciClient/include/Networking/LevelWatchingManager.hpp"
#include "ViciClient/include/Entities/ClientPlayer.hpp"
#include "ViciClient/include/JSRuntime/ClientScriptLoader.hpp"

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