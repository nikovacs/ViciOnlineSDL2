#pragma once

#include <unordered_map>
#include <memory>
#include <SDL2/SDL.h>
#include <mutex>
#include "ViciClient/include/Entities/NetworkedPlayer.hpp"

namespace Networking {
	class ClientPlayerManager {
	public:
		ClientPlayerManager() = delete;
		static void update();
		static void render(SDL_Renderer* renderer);

		static void spawnPlayer(nlohmann::json& json);
		static void despawnPlayer(SimplePacket& packet);
		static void updatePlayerPos(SimplePacket& packet);
		static void updatePlayerAni(SimplePacket& packet);
		static void updatePlayerDir(SimplePacket& packet);
		static Entities::NetworkedPlayer* getPlayer(std::string_view username);
	private:
		static std::unordered_map<uint32_t, std::unique_ptr<Entities::NetworkedPlayer>> _players;
		static std::unordered_map<std::string, uint32_t> _playerUsernamesToIds;
		static std::mutex _playerMutex;
	};
}