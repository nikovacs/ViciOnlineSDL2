#pragma once

#include <unordered_map>
#include <memory>
#include "NetworkedPlayer.hpp"
#include <SDL2/SDL.h>
#include <mutex>

namespace Networking {
	class ClientPlayerManager {
	public:
		ClientPlayerManager() = delete;
		static void update();
		static void render(SDL_Renderer* renderer);

		static void spawnPlayer(nlohmann::json& json);
		static void despawnPlayer(nlohmann::json& json);
		static void updatePlayerPos(nlohmann::json& json);
		static void updatePlayerAni(nlohmann::json& json);
		static void updatePlayerDir(nlohmann::json& json);
		static Entities::NetworkedPlayer* getPlayer(std::string_view username);
	private:
		static std::unordered_map<uint32_t, std::unique_ptr<Entities::NetworkedPlayer>> _players;
		static std::unordered_map<std::string, uint32_t> _playerUsernamesToIds;
		static std::mutex _playerMutex;
	};
}