#include "ServerPlayerManager.hpp"
#include "ViciServer.hpp"
#include <string>
#include "../ViciEngine/UdpChannels.hpp"
#include "enet/enet.h"
#include <memory>
#include "ServerScriptLoader.hpp"

namespace Networking {
	std::unordered_map<uint32_t, std::unique_ptr<Entities::ServerPlayer>> ServerPlayerManager::_players{};
	std::unordered_map<uint32_t, ENetPeer*> ServerPlayerManager::_peers{};
	std::recursive_mutex ServerPlayerManager::_playerMutex{};

	std::unordered_map<std::string, std::set<uint32_t>> ServerPlayerManager::_playersOnLevel{};
	std::unordered_map<std::string, std::set<uint32_t>> ServerPlayerManager::_playersWatchingLevel{};

	void ServerPlayerManager::sendInitialPlayerData(ENetPeer* peer, nlohmann::json& json) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);

		_peers.emplace(peer->connectID, peer);
		
		nlohmann::json& serverOptions = ViciServer::instance->getServerOptions();
		
		std::string username = json["usr"];
		std::string playerId = json["id"];
		std::string fileName = "playerData/" + playerId + ".json";

		// check if file exists
		nlohmann::json playerData;
		bool fileExists{ std::filesystem::exists(fileName) };
		std::cout << "file exists (" << fileName << "): " << fileExists << "\n";
		if (fileExists) {
			std::ifstream in(fileName);
			std::stringstream buffer;
			buffer << in.rdbuf();
			playerData = nlohmann::json::parse(buffer.str());
		}
		else {
			playerData["x"] = serverOptions["defaultX"];
			playerData["y"] = serverOptions["defaultY"];
			playerData["w"] = serverOptions["defaultWidth"];
			playerData["h"] = serverOptions["defaultHeight"];
			playerData["dir"] = serverOptions["defaultDir"];
			playerData["animation"] = serverOptions["defaultAni"];
			playerData["world"] = serverOptions["defaultWorld"];
			playerData["cameraZoom"] = serverOptions["defaultZoom"];
			playerData["clientW"] = serverOptions["defaultClientW"];
			playerData["clientR"] = serverOptions["defaultClientR"];
		}

		nlohmann::json* clientW = new nlohmann::json( playerData["clientW"] );
		nlohmann::json* clientR = new nlohmann::json( playerData["clientR"] );

		// takes ownership of clientW and clientR pointers
		_players.emplace(peer->connectID, std::make_unique<Entities::ServerPlayer>(
			username, playerId, peer->connectID, playerData["animation"], playerData["world"], playerData["dir"], playerData["x"], playerData["y"], playerData["cameraZoom"], clientW, clientR
		));
		Entities::ServerPlayer& player = *_players.at(peer->connectID).get();
		player.setWidth(playerData["w"]);
		player.setHeight(playerData["h"]);

		UdpServer::sendJson(peer, playerData, Networking::UdpChannels::initialPlayerData, ENET_PACKET_FLAG_RELIABLE);
	}

	void ServerPlayerManager::spawnPlayer(uint32_t idToSpawn, uint32_t spawnForId) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		
		Entities::ServerPlayer* playerToSpawn = _players.at(idToSpawn).get();
		
		nlohmann::json playerData{};
		playerData["id"] = idToSpawn;
		playerData["usr"] = playerToSpawn->getUsername();
		playerData["x"] = playerToSpawn->getX();
		playerData["y"] = playerToSpawn->getY();
		playerData["w"] = playerToSpawn->getWidth();
		playerData["h"] = playerToSpawn->getHeight();
		playerData["dir"] = playerToSpawn->getDir();
		playerData["animation"] = playerToSpawn->getAni();
		playerData["clientW"] = *(playerToSpawn->getClientW().getUnderlyingJson());
		playerData["clientR"] = *(playerToSpawn->getClientR().getUnderlyingJson());

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

		std::set<uint32_t>& players{ _getPlayersWatchingLevel(_players.at(id)->getLevel()) };
		for (uint32_t pId : players) {
			UdpServer::sendJson(_peers.at(pId), playerData, UdpChannels::DespawnPlayer, ENET_PACKET_FLAG_RELIABLE);
		}

		Entities::ServerPlayer& player = *_players.at(id);
		for (const std::string& lvl : player.getLevelsWatching()) {
			_playersWatchingLevel.at(lvl).erase(id);
		}
		_playersOnLevel.at(player.getLevel().data()).erase(id);
		_players.erase(id); // player reference is now invalid
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
		std::set<uint32_t>& players{ _getPlayersWatchingLevel(level) };
		for (uint32_t pId : players) {
			if (pId == id) continue;
			UdpServer::sendJson(_peers.at(pId), json, UdpChannels::UpdatePlayerPos, ENET_PACKET_FLAG_UNSEQUENCED);
		}
	}
	
	void ServerPlayerManager::updatePlayerAni(uint32_t id, nlohmann::json& json) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		
		_players.at(id)->setAni(json["ani"]);
		json["id"] = id;

		std::string_view level{ _players.at(id)->getLevel() };
		std::set<uint32_t>& players{ _getPlayersWatchingLevel(level) };
		for (uint32_t pId : players) {
			if (pId == id) continue;
			UdpServer::sendJson(_peers.at(pId), json, UdpChannels::UpdatePlayerAni, ENET_PACKET_FLAG_RELIABLE);
		}
	}

	void ServerPlayerManager::updatePlayerDir(uint32_t id, nlohmann::json& json) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;

		_players.at(id)->setDir(json["dir"]);
		json["id"] = id;
		
		std::string_view level{ _players.at(id)->getLevel() };
		std::set<uint32_t>& players{ _getPlayersWatchingLevel(level) };
		for (uint32_t pId : players) {
			if (pId == id) continue;
			UdpServer::sendJson(_peers.at(pId), json, UdpChannels::UpdatePlayerDir, ENET_PACKET_FLAG_RELIABLE);
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
		if (!_playersOnLevel.contains(levelName.data())) return;
		
		std::set<uint32_t>& levelSet{ _playersOnLevel.at(levelName.data()) };
		levelSet.erase(id);
		if (levelSet.empty()) {
			_playersOnLevel.erase(levelName.data()); // levelSet ref invalid here
		}
	}

	void ServerPlayerManager::startWatchingLevel(uint32_t id, nlohmann::json& json) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		std::string_view levelName = json["lvl"];
		if (!_playersWatchingLevel.contains(levelName.data())) {
			_playersWatchingLevel.emplace(levelName, std::set<uint32_t>{});
		}
		_playersWatchingLevel.at(levelName.data()).insert(id);
		_players.at(id)->startWatchingLevel(levelName);

		// spawn the players on that level for the client watching that level
		std::set<uint32_t>& players{ _getPlayersOnLevel(levelName) };
		for (uint32_t pId : players) {
			if (pId == id) continue;
			spawnPlayer(pId, id);
		}
	}

	void ServerPlayerManager::stopWatchingLevel(uint32_t id, nlohmann::json& json) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		std::string_view levelName = json["lvl"];
		if (!_playersWatchingLevel.contains(levelName.data())) return;
		std::set<uint32_t>& levelSet{ _playersWatchingLevel.at(levelName.data()) };
		levelSet.erase(id);
		_players.at(id)->stopWatchingLevel(levelName);

		std::set<uint32_t>& players{ _getPlayersOnLevel(levelName) };
		for (uint32_t pId : players) {
			if (pId == id) continue;
			despawnPlayer(pId, id);
		}

		if (levelSet.empty()) {
			_playersWatchingLevel.erase(levelName.data());
		}
	}

	void ServerPlayerManager::updatePlayerLevel(uint32_t id, nlohmann::json& json) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		std::string_view newLevel = json["lvl"];
		std::string oldLevel{ _players.at(id)->getLevel() };
		_players.at(id)->setLevel(newLevel);

		removeFromLevel(id, oldLevel);
		addToLevel(id, newLevel);
		
		std::set<uint32_t>& pIdsWatchingNewLevel{ _getPlayersWatchingLevel(newLevel) };
		if (oldLevel.empty()) {
			for (uint32_t pId : pIdsWatchingNewLevel) {
				if (pId == id) continue;
				spawnPlayer(id, pId);
			}
			return;
		}
		
		std::set<uint32_t>& pIdsWatchingOldLevel{ _getPlayersWatchingLevel(oldLevel) };
		
		std::set<uint32_t> spawnForPlayersIds{};
		std::set_difference(pIdsWatchingNewLevel.begin(), pIdsWatchingNewLevel.end(), pIdsWatchingOldLevel.begin(), pIdsWatchingOldLevel.end(), std::inserter(spawnForPlayersIds, spawnForPlayersIds.begin()));
		std::set<uint32_t> despawnForPlayerIds{};
		std::set_difference(pIdsWatchingOldLevel.begin(), pIdsWatchingOldLevel.end(), pIdsWatchingNewLevel.begin(), pIdsWatchingNewLevel.end(), std::inserter(despawnForPlayerIds, despawnForPlayerIds.begin()));
		
		for (uint32_t pId : spawnForPlayersIds) {
			if (pId == id) continue;
			spawnPlayer(id, pId);
		}

		for (uint32_t pId : despawnForPlayerIds) {
			if (pId == id) continue;
			despawnPlayer(id, pId);
		}
	}

	void ServerPlayerManager::updatePlayerClientW(uint32_t id, nlohmann::json& json) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		
		std::cout << "updating player attr for id " << id << "\n";

		if (!_players.contains(id)) return;
		_players.at(id)->getClientW().set<nlohmann::json&>(json["k"], json["v"]);

		json["id"] = id;

		std::string_view level{ _players.at(id)->getLevel() };
		std::set<uint32_t>& players{ _getPlayersWatchingLevel(level) };
		for (uint32_t pId : players) {
			if (pId == id) continue;
			UdpServer::sendJson(_peers.at(pId), json, UdpChannels::UpdateClientW, ENET_PACKET_FLAG_RELIABLE);
		}
	}

	void ServerPlayerManager::updatePlayerCameraZoom(uint32_t id, nlohmann::json& json) {
		std::lock_guard<std::recursive_mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players.at(id)->setCameraZoom(json["cameraZoom"]);
	}

	std::set<uint32_t>& ServerPlayerManager::_getPlayersOnLevel(std::string_view levelName) {
		if (_playersOnLevel.contains(levelName.data())) {
			return _playersOnLevel.at(levelName.data());
		}
		static std::set<uint32_t> emptySet{};
		return emptySet;
	}

	std::set<uint32_t>& ServerPlayerManager::_getPlayersWatchingLevel(std::string_view levelName) {
		if (_playersWatchingLevel.contains(levelName.data())) {
			return _playersWatchingLevel.at(levelName.data());
		}
		static std::set<uint32_t> emptySet{};
		return emptySet;
	}
}