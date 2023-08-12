#include "ClientPlayer.hpp"
#include "nlohmann/json.hpp"
#include "enet/enet.h"
#include "UdpClient.hpp"

namespace Entities {
	ClientPlayer::ClientPlayer(std::string_view aniName, int x, int y, int direction) : EntityAnimated(aniName, x, y, direction) {};

	void ClientPlayer::setDir(int dir) {
		Entity::setDir(dir);
		// TODO send packet to server
	}

	void ClientPlayer::setAniHard(std::string_view aniName) {
		EntityAnimated::setAniHard(aniName);
		// TODO send packet to server
	}

	void ClientPlayer::setPosition(int x, int y) {
		EntityAnimated::setPosition(x, y);
		nlohmann::json json{};
		json["x"] = _x;
		json["y"] = _y;
		Networking::UdpClient::sendJson(json, Networking::UdpChannels::UpdatePlayerPos, ENET_PACKET_FLAG_UNSEQUENCED);
	}
}