#pragma once

#include <unordered_map>
#include "ServerPlayer.hpp"
#include <memory>
#include <enet/enet.h>

namespace Networking {
	class PlayerManager {
	public:
		PlayerManager() = delete;
		static void sendInitialPlayerData(ENetPeer* peer);
		static void spawnPlayer(uint32_t id);
		static void despawnPlayer(uint32_t id);
		static void updatePlayerPos(uint32_t id);
		static void updatePlayerAniHard(uint32_t id);
		static void updatePlayerAniSoft(uint32_t id);
		static void updatePlayerDir(uint32_t id);
	private:
		static std::unordered_map<uint32_t, std::unique_ptr<Entities::ServerPlayer>> _players;
		static std::unordered_map<uint32_t, ENetPeer*> _peers;
		static std::mutex _playerMutex;
	};
}