#pragma once

#include <unordered_map>
#include <nlohmann/json.hpp>
#include "ServerPlayer.hpp"
#include <memory>
#include <enet/enet.h>
#include "../ViciEngine/SimplePacket.hpp"
#include <mutex>

namespace Networking {
	class ServerPlayerManager {
	public:
		ServerPlayerManager() = delete;
		static void sendInitialPlayerData(ENetPeer* peer, SimplePacket& packet);
		static void spawnPlayer(uint32_t idToSpawn, uint32_t spawnForId);
		static void despawnPlayer(uint32_t idToDespawn, uint32_t despawnForId);
		static void onPlayerDisconnect(uint32_t id);
		static void updatePlayerPos(uint32_t id, SimplePacket& packet);
		static void updatePlayerAni(uint32_t id, SimplePacket& packet);
		static void updatePlayerDir(uint32_t id, SimplePacket& packet);
		static void addToLevel(uint32_t id, std::string_view levelName);
		static void removeFromLevel(uint32_t id, std::string_view levelName);
		static void startWatchingLevel(uint32_t id, SimplePacket& packet);
		static void stopWatchingLevel(uint32_t id, SimplePacket& packet);
		static void updatePlayerLevel(uint32_t id, SimplePacket& packet);
		static void updatePlayerClientW(uint32_t id, nlohmann::json& json);
		static void updatePlayerCameraZoom(uint32_t id, SimplePacket& packet);
		static inline std::vector<uint32_t> getPlayersOnLevel(std::string_view levelName) {
			std::lock_guard<std::recursive_mutex> lock(_playerMutex);
			if (!_playersOnLevel.contains(levelName.data())) return {};
			auto playerSet = _playersOnLevel.at(levelName.data());
			return { playerSet.begin(), playerSet.end() };
		}
		static inline std::vector<uint32_t> getPlayersWatchingLevel(std::string_view levelName) {
			std::lock_guard<std::recursive_mutex> lock(_playerMutex);
			if (!_playersWatchingLevel.contains(levelName.data())) return {};
			auto playerSet = _playersWatchingLevel.at(levelName.data());
			return { playerSet.begin(), playerSet.end() };
		}
		static Entities::ServerPlayer* getPlayer(uint32_t playerId);
	private:
		static std::unordered_map<uint32_t, std::unique_ptr<Entities::ServerPlayer>> _players;
		static std::unordered_map<uint32_t, ENetPeer*> _peers;
		static std::recursive_mutex _playerMutex;

		static std::unordered_map<std::string, std::set<uint32_t>> _playersOnLevel;
		static std::unordered_map<std::string, std::set<uint32_t>> _playersWatchingLevel;

		static std::set<uint32_t>& _getPlayersOnLevel(std::string_view levelName);
		static std::set<uint32_t>& _getPlayersWatchingLevel(std::string_view levelName);
	};
}