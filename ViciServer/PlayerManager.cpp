#include "PlayerManager.hpp"
#include "nlohmann/json.hpp"
#include "ViciServer.hpp"
#include <string>
#include "../ViciEngine/UdpChannels.hpp"
#include "enet/enet.h"

namespace Networking {
	std::unordered_map<uint32_t, std::unique_ptr<Entities::ServerPlayer>> PlayerManager::_players{};
	std::unordered_map<uint32_t, ENetPeer*> PlayerManager::_peers{};
	std::mutex PlayerManager::_playerMutex{};

	void PlayerManager::sendInitialPlayerData(ENetPeer* peer) {
		std::lock_guard<std::mutex> lock(_playerMutex);

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
		_players.at(peer->connectID)->setWidth(playerData["w"]);
		_players.at(peer->connectID)->setHeight(playerData["h"]);

		std::string jsonString{ playerData.dump() };
		
		ENetPacket* packet = enet_packet_create(jsonString.c_str(), jsonString.length() + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, Networking::UdpChannels::initialPlayerData, packet);
		spawnPlayer(peer->connectID);
		enet_host_flush(Networking::UdpServer::instance->getHost());
	}

	void PlayerManager::spawnPlayer(uint32_t id) {
		auto* playerToSpawn = _players.at(id).get();
		nlohmann::json playerData{};
		playerData["id"] = id;
		playerData["x"] = playerToSpawn->getX();
		playerData["y"] = playerToSpawn->getY();
		playerData["w"] = playerToSpawn->getWidth();
		playerData["h"] = playerToSpawn->getHeight();
		playerData["dir"] = playerToSpawn->getDir();
		playerData["animation"] = playerToSpawn->getAni();

		std::string jsonString{ playerData.dump() };
		
		for (auto& p : _players) { // TODO eventually only spawn for players who should be able to see the player
			if (p.first == id) continue;
			ENetPeer* peer{ _peers.at(p.first) };
			ENetPacket* packet = enet_packet_create(jsonString.c_str(), jsonString.length() + 1, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, Networking::UdpChannels::SpawnPlayer, packet);

			Entities::ServerPlayer* otherPlayerToSpawn{ _players.at(p.first).get() };
			nlohmann::json otherPlayerData{};
			otherPlayerData["id"] = p.first;
			otherPlayerData["x"] = otherPlayerToSpawn->getX();
			otherPlayerData["y"] = otherPlayerToSpawn->getY();
			otherPlayerData["w"] = otherPlayerToSpawn->getWidth();
			otherPlayerData["h"] = otherPlayerToSpawn->getHeight();
			otherPlayerData["dir"] = otherPlayerToSpawn->getDir();
			otherPlayerData["animation"] = otherPlayerToSpawn->getAni();

			std::string otherJsonString{ otherPlayerData.dump() };

			peer = _peers.at(id);
			ENetPacket* packet2 = enet_packet_create(otherJsonString.c_str(), otherJsonString.length() + 1, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, Networking::UdpChannels::SpawnPlayer, packet2);
		}
	}
	void PlayerManager::despawnPlayer(uint32_t id) {
		nlohmann::json json{};
		json["id"] = id;
		std::string jsonString{ json.dump() };
		
		for (auto& p : _players) {
			if (p.first == id) continue;
			ENetPeer* peer{ _peers.at(p.first) };
			
			ENetPacket* packet = enet_packet_create(jsonString.c_str(), jsonString.length() + 1, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, Networking::UdpChannels::DespawnPlayer, packet);
		}
		
		_players.erase(id);
		_peers.erase(id);
	}
	void PlayerManager::updatePlayerPos(uint32_t id) {
		std::lock_guard<std::mutex> lock(_playerMutex);
	
	}
	void PlayerManager::updatePlayerAniHard(uint32_t id) {
		std::lock_guard<std::mutex> lock(_playerMutex);
	
	}
	void PlayerManager::updatePlayerAniSoft(uint32_t id) {
		std::lock_guard<std::mutex> lock(_playerMutex);
	
	}
	void PlayerManager::updatePlayerDir(uint32_t id) {
		std::lock_guard<std::mutex> lock(_playerMutex);
	
	}
}