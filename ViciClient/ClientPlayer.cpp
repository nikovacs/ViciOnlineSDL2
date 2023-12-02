#include "ClientPlayer.hpp"
#include "nlohmann/json.hpp"
#include "enet/enet.h"
#include "UdpClient.hpp"
#include <iostream>

namespace Entities {
	ClientPlayer::ClientPlayer(nlohmann::json& json) : EntityAnimated(json["animation"], json["x"], json["y"], json["dir"], new nlohmann::json(json["clientW"]), new nlohmann::json(json["clientR"])) {
		_clientW.setOnSetAttribCallback([this](std::string_view key) {
			nlohmann::json json{};
			json["k"] = key;
			json["v"] = _clientW.get(key);
			std::cout << "Updating clientw attrs with " << key << " and " << _clientW.get(key) << std::endl;
			Networking::UdpClient::sendJson(json, Networking::UdpChannels::UpdateClientW, ENET_PACKET_FLAG_RELIABLE);
		});
	};

	void ClientPlayer::setDir(int dir) {
		if (dir == _dir) return;
		Entity::setDir(dir);
		nlohmann::json json{};
		json["dir"] = dir;
		Networking::UdpClient::sendJson(json, Networking::UdpChannels::UpdatePlayerDir, ENET_PACKET_FLAG_RELIABLE);
	}

	void ClientPlayer::setAniHard(std::string_view aniName) {
		EntityAnimated::setAniHard(aniName);
		nlohmann::json json{};
		json["ani"] = aniName;
		Networking::UdpClient::sendJson(json, Networking::UdpChannels::UpdatePlayerAni, ENET_PACKET_FLAG_RELIABLE);
	}

	void ClientPlayer::setPosition(int x, int y) {
		EntityAnimated::setPosition(x, y);
		nlohmann::json json{};
		json["x"] = _x;
		json["y"] = _y;
		Networking::UdpClient::sendJson(json, Networking::UdpChannels::UpdatePlayerPos, ENET_PACKET_FLAG_UNSEQUENCED);
	}
}