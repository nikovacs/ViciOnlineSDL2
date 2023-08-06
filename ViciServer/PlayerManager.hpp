#pragma once

#include <unordered_map>
#include "ServerPlayer.hpp"
#include <memory>

namespace Networking {
	class PlayerManager {
	public:
		PlayerManager() = delete;
		static void spawnPlayer();
		static void despawnPlayer();
		static void updatePlayerPos();
		static void updatePlayerAniHard();
		static void updatePlayerAniSoft();
		static void updatePlayerDir();
	private:
		static std::unordered_map<uint32_t, std::unique_ptr<Entities::ServerPlayer>> _players;
		static std::mutex _playerMutex;
	};
}