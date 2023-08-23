#include "ServerPlayerManager.hpp"
#include "ViciServer.hpp"
#include <string>
#include "../ViciEngine/UdpChannels.hpp"
#include "enet/enet.h"

namespace Networking {
	std::unordered_map<uint32_t, std::unique_ptr<Entities::ServerPlayer>> ServerPlayerManager::_players{};
	std::unordered_map<uint32_t, ENetPeer*> ServerPlayerManager::_peers{};
	std::recursive_mutex ServerPlayerManager::_playerMutex{};

	std::unordered_map<std::string, std::set<uint32_t>> ServerPlayerManager::_playersOnLevel = {};
	std::unordered_map<std::string, std::set<uint32_t>> ServerPlayerManager::_playersWatchingLevel = {};

	void ServerPlayerManager::sendInitialPlayerData(ENetPeer* peer) {
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
		playerData["world"] = serverOptions["defaultWorld"];
		playerData["cameraZoom"] = serverOptions["defaultZoom"];

		_players.emplace(peer->connectID, std::make_unique<Entities::ServerPlayer>(peer->connectID, playerData["animation"], playerData["world"], playerData["dir"], playerData["x"], playerData["y"]));
		Entities::ServerPlayer& player = *_players.at(peer->connectID).get();
		player.setWidth(playerData["w"]);
		player.setHeight(playerData["h"]);

		UdpServer::sendJson(peer, playerData, Networking::UdpChannels::initialPlayerData, ENET_PACKET_FLAG_RELIABLE);
		
		std::vector<uint32_t> players{ getPlayersWatchingLevel(player.getLevel()) };
		for (uint32_t pId : players) {
			if (pId == peer->connectID) continue;
			spawnPlayer(peer->connectID, pId);
		}
	}

	void ServerPlayerManager::spawnPlayer(uint32_t idToSpawn, uint32_t spawnForId) {
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
	
	void ServerPlayerManager::despawnPlayer(uint32_t idToDespawn, uint32_t despawnForId) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		
		nlohmann::json playerData{};
		playerData["id"] = idToDespawn;

		UdpServer::sendJson(_peers.at(despawnForId), playerData, UdpChannels::DespawnPlayer, ENET_PACKET_FLAG_RELIABLE);
	}
	
	void ServerPlayerManager::onPlayerDisconnect(uint32_t id) {
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
	
	void ServerPlayerManager::updatePlayerPos(uint32_t id, nlohmann::json& json) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		
		int newX = json["x"];
		int newY = json["y"];
		_players.at(id)->setPosition(newX, newY);

		json["id"] = id;
		
		std::string_view level{ _players.at(id)->getLevel() };
		std::vector<uint32_t> players{ getPlayersWatchingLevel(level) };
		for (uint32_t pId : players) {
			if (pId == id) continue;
			UdpServer::sendJson(_peers.at(pId), json, UdpChannels::UpdatePlayerPos, ENET_PACKET_FLAG_UNSEQUENCED);
		}
	}
	
	void ServerPlayerManager::updatePlayerAni(uint32_t id, nlohmann::json& json) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		
		_players.at(id)->setAni(json["level"]);
		json["id"] = id;

		std::string_view level{ _players.at(id)->getLevel() };
		std::vector<uint32_t> players{ getPlayersWatchingLevel(level) };
		for (uint32_t pId : players) {
			if (pId == id) continue;
			UdpServer::sendJson(_peers.at(pId), json, UdpChannels::UpdatePlayerPos, ENET_PACKET_FLAG_UNSEQUENCED);
		}
	}

	void ServerPlayerManager::updatePlayerDir(uint32_t id, nlohmann::json& json) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;

		_players.at(id)->setDir(json["dir"]);
		json["id"] = id;
		
		std::string_view level{ _players.at(id)->getLevel() };
		std::vector<uint32_t> players{ getPlayersWatchingLevel(level) };
		for (uint32_t pId : players) {
			if (pId == id) continue;
			UdpServer::sendJson(_peers.at(pId), json, UdpChannels::UpdatePlayerPos, ENET_PACKET_FLAG_UNSEQUENCED);
		}
	}

	void ServerPlayerManager::addToLevel(uint32_t id, std::string_view levelName) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		if (!_playersOnLevel.contains(levelName.data())) {
			_playersOnLevel.emplace(levelName, std::set<uint32_t>{});
		}
		_playersOnLevel.at(levelName.data()).insert(id);
	}

	void ServerPlayerManager::removeFromLevel(uint32_t id, std::string_view levelName) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		if (!_playersOnLevel.contains(levelName.data())) return; // consider removing empty set?
		_playersOnLevel.at(levelName.data()).erase(id);
	}

	void ServerPlayerManager::startWatchingLevel(uint32_t id, nlohmann::json& json) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		std::string_view levelName = json["level"];
		if (!_playersWatchingLevel.contains(levelName.data())) {
			_playersWatchingLevel.emplace(levelName, std::set<uint32_t>{});
		}
		_playersWatchingLevel.at(levelName.data()).insert(id);

		// spawn the players on that level for the client watching that level
		std::vector<uint32_t> players{ getPlayersOnLevel(levelName) };
		for (uint32_t pId : players) {
			if (pId == id) continue;
			spawnPlayer(pId, id);
		}
	}

	void ServerPlayerManager::stopWatchingLevel(uint32_t id, nlohmann::json& json) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		std::string_view levelName = json["level"];
		if (!_playersWatchingLevel.contains(levelName.data())) return; // consider removing empty set?
		_playersWatchingLevel.at(levelName.data()).erase(id);

		std::vector<uint32_t> players{ getPlayersOnLevel(levelName) };
		for (uint32_t pId : players) {
			if (pId == id) continue;
			despawnPlayer(pId, id);
		}
	}
}