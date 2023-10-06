#include "ClientPlayer.hpp"
#include "nlohmann/json.hpp"
#include "enet/enet.h"
#include "UdpClient.hpp"
#include <iostream>

namespace Entities {
	ClientPlayer::ClientPlayer(nlohmann::json& json) : EntityAnimated(json["animation"], json["x"], json["y"], json["dir"]), _clientWriteableAttrs{ json["clientw"] } {
		_clientWriteableAttrs.setOnSetAttribCallback([this](std::string_view key) {
			nlohmann::json json{};
			json["k"] = key;
			json["v"] = _clientWriteableAttrs.get(key);
			std::cout << "Updating clientw attrs with " << key << " and " << _clientWriteableAttrs.get(key) << std::endl;
			Networking::UdpClient::sendJson(json, Networking::UdpChannels::UpdatePlayerAttr, ENET_PACKET_FLAG_RELIABLE);
		});
	};

	void ClientPlayer::setDir(int dir) {
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