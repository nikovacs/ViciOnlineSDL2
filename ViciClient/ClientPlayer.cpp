#include "ClientPlayer.hpp"
#include "nlohmann/json.hpp"
#include "enet/enet.h"
#include "UdpClient.hpp"

namespace Entities {
	ClientPlayer::ClientPlayer(std::string_view aniName, int x, int y, int direction) : EntityAnimated(aniName, x, y, direction) {};

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