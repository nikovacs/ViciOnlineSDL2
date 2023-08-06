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

		std::string jsonString{ playerData.dump() };
		
		ENetPacket* packet = enet_packet_create(jsonString.c_str(), jsonString.length() + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, Networking::UdpChannels::initialPlayerData, packet);
		enet_host_flush(Networking::UdpServer::instance->getHost());
	}

	void PlayerManager::spawnPlayer(uint32_t id) {
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (_players.contains(id)) return;
		// enet logic
	}
	void PlayerManager::despawnPlayer(uint32_t id) {
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players.erase(id);
		// enet logic
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