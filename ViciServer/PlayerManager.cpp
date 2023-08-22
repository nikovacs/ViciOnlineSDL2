#include "PlayerManager.hpp"
#include "nlohmann/json.hpp"
#include "ViciServer.hpp"
#include <string>
#include "../ViciEngine/UdpChannels.hpp"
#include "enet/enet.h"

namespace Networking {
	std::unordered_map<uint32_t, std::unique_ptr<Entities::ServerPlayer>> PlayerManager::_players{};
	std::unordered_map<uint32_t, ENetPeer*> PlayerManager::_peers{};
	std::recursive_mutex PlayerManager::_playerMutex{};

	std::unordered_map<std::string, std::set<uint32_t>> PlayerManager::_playersOnLevel = {};
	std::unordered_map<std::string, std::set<uint32_t>> PlayerManager::_playersWatchingLevel = {};
	std::mutex PlayerManager::_lvlMtx = {};

	void PlayerManager::sendInitialPlayerData(ENetPeer* peer) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);

		_peers.emplace(peer->connectID, peer);
		
		nlohmann::json& serverOptions = ViciServer::instance->getServerOptions();
		// TODO check if player's last info is saved in a file and load it

		nlohmann::json playerData{};
		playerData["x"] = serverOptions["defaultX"];
		playerData["y"] = serverOptions["defaultY"];
		playerData["w"] = serverOptions["defaultWidth"];
		playerData["h"] = serverOptions["defaultHeight"];
		playerData["dir"] = serverOptions["defaultDir"];
		playerData["animation"] = serverOptions["defaultAni"];
		playerData["level"] = serverOptions["defaultLevel"];
		playerData["cameraZoom"] = serverOptions["defaultZoom"];

		_players.emplace(peer->connectID, std::make_unique<Entities::ServerPlayer>(peer->connectID, playerData["animation"], playerData["level"], playerData["dir"], playerData["x"], playerData["y"]));
		Entities::ServerPlayer* player = _players.at(peer->connectID).get();
		player->setWidth(playerData["w"]);
		player->setHeight(playerData["h"]);

		UdpServer::sendJson(peer, playerData, Networking::UdpChannels::initialPlayerData, ENET_PACKET_FLAG_RELIABLE);
		
		std::vector<uint32_t> players{ getPlayersWatchingLevel(player->getLevel()) };
		for (uint32_t pId : players) {
			spawnPlayer(peer->connectID, pId);
		}
	}

	void PlayerManager::spawnPlayer(uint32_t idToSpawn, uint32_t spawnForId) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		
		Entities::ServerPlayer* playerToSpawn = _players.at(idToSpawn).get();
		
		nlohmann::json playerData{};
		playerData["id"] = idToSpawn;
		playerData["x"] = playerToSpawn->getX();
		playerData["y"] = playerToSpawn->getY();
		playerData["w"] = playerToSpawn->getWidth();
		playerData["h"] = playerToSpawn->getHeight();
		playerData["dir"] = playerToSpawn->getDir();
		playerData["animation"] = playerToSpawn->getAni();

		UdpServer::sendJson(_peers.at(spawnForId), playerData, UdpChannels::SpawnPlayer, ENET_PACKET_FLAG_RELIABLE);
	}
	void PlayerManager::despawnPlayer(uint32_t id) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		
		nlohmann::json playerData{};
		playerData["id"] = id;

		std::vector<uint32_t> players{ getPlayersWatchingLevel(_players.at(id)->getLevel()) };
		for (uint32_t pId : players) {
			UdpServer::sendJson(_peers.at(pId), playerData, UdpChannels::DespawnPlayer, ENET_PACKET_FLAG_RELIABLE);
		}

		// TODO, store level watching info on the player object so we can use it here to remove from everything in PlayerLevelManager
		
		_players.erase(id);
		_peers.erase(id);
	}
	void PlayerManager::updatePlayerPos(uint32_t id) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
	
	}
	void PlayerManager::updatePlayerAniHard(uint32_t id) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
	
	}
	void PlayerManager::updatePlayerAniSoft(uint32_t id) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
	
	}
	void PlayerManager::updatePlayerDir(uint32_t id) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
	
	}

	void PlayerManager::addToLevel(uint32_t id, std::string_view levelName) {
		std::lock_guard<std::mutex> lock(_lvlMtx);
		if (!_playersOnLevel.contains(levelName.data())) {
			_playersOnLevel.emplace(levelName, std::set<uint32_t>{});
		}
		_playersOnLevel.at(levelName.data()).insert(id);
	}

	void PlayerManager::removeFromLevel(uint32_t id, std::string_view levelName) {
		std::lock_guard<std::mutex> lock(_lvlMtx);
		if (!_playersOnLevel.contains(levelName.data())) return; // consider removing empty set?
		_playersOnLevel.at(levelName.data()).erase(id);
	}

	void PlayerManager::startWatchingLevel(uint32_t id, std::string_view levelName) {
		std::lock_guard<std::mutex> lock(_lvlMtx);
		if (!_playersWatchingLevel.contains(levelName.data())) {
			_playersWatchingLevel.emplace(levelName, std::set<uint32_t>{});
		}
		_playersWatchingLevel.at(levelName.data()).insert(id);
	}

	void PlayerManager::stopWatchingLevel(uint32_t id, std::string_view levelName) {
		std::lock_guard<std::mutex> lock(_lvlMtx);
		if (!_playersWatchingLevel.contains(levelName.data())) return; // consider removing empty set?
		_playersWatchingLevel.at(levelName.data()).erase(id);
	}
}