#pragma once

#include <unordered_map>
#include "ServerPlayer.hpp"
#include <memory>
#include <enet/enet.h>
#include <mutex>

namespace Networking {
	class PlayerManager {
	public:
		PlayerManager() = delete;
		static void sendInitialPlayerData(ENetPeer* peer);
		static void spawnPlayer(uint32_t idToSpawn, uint32_t spawnForId);
		static void despawnPlayer(uint32_t id);
		static void updatePlayerPos(uint32_t id);
		static void updatePlayerAniHard(uint32_t id);
		static void updatePlayerAniSoft(uint32_t id);
		static void updatePlayerDir(uint32_t id);
		static void addToLevel(uint32_t id, std::string_view levelName);
		static void removeFromLevel(uint32_t id, std::string_view levelName);
		static void startWatchingLevel(uint32_t id, std::string_view levelName);
		static void stopWatchingLevel(uint32_t id, std::string_view levelName);
		static inline std::vector<uint32_t> getPlayersOnLevel(std::string_view levelName) {
			std::lock_guard<std::mutex> lock(_lvlMtx);
			if (!_playersOnLevel.contains(levelName.data())) return {};
			auto playerSet = _playersOnLevel.at(levelName.data());
			return { playerSet.begin(), playerSet.end() };
		}
		static inline std::vector<uint32_t> getPlayersWatchingLevel(std::string_view levelName) {
			std::lock_guard<std::mutex> lock(_lvlMtx);
			if (!_playersWatchingLevel.contains(levelName.data())) return {};
			auto playerSet = _playersWatchingLevel.at(levelName.data());
			return { playerSet.begin(), playerSet.end() };
		}
	private:
		static std::unordered_map<uint32_t, std::unique_ptr<Entities::ServerPlayer>> _players;
		static std::unordered_map<uint32_t, ENetPeer*> _peers;
		static std::recursive_mutex _playerMutex;

		static std::unordered_map<std::string, std::set<uint32_t>> _playersOnLevel;
		static std::unordered_map<std::string, std::set<uint32_t>> _playersWatchingLevel;
		static std::mutex _lvlMtx;
	};
}