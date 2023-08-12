#include "PlayerManager.hpp"
#include "nlohmann/json.hpp"
#include "ViciServer.hpp"
#include <string>
#include "../ViciEngine/UdpChannels.hpp"
#include "enet/enet.h"
#include "PlayerLevelManager.hpp"

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

		UdpServer::sendJson(peer, playerData, Networking::UdpChannels::initialPlayerData, ENET_PACKET_FLAG_RELIABLE);
		spawnPlayer(peer->connectID);
	}

	void PlayerManager::spawnPlayer(uint32_t id) {

	}
	void PlayerManager::despawnPlayer(uint32_t id) {

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