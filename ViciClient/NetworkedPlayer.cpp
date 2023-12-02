#include "NetworkedPlayer.hpp"

namespace Entities {
	NetworkedPlayer::NetworkedPlayer(std::string_view username, std::string_view aniName, int x, int y, int direction, nlohmann::json* clientW, nlohmann::json* clientR)
		: EntityAnimated(aniName, x, y, direction, clientW, clientR), _username{username} {
		_clientW.setOnSetAttribCallback([this](std::string_view key)->void {
			nlohmann::json json{};
			json["k"] = key;
			json["v"] = _clientW.get(key);

			Networking::UdpClient::sendJson(json, Networking::UdpChannels::UpdateClientW, ENET_PACKET_FLAG_RELIABLE);
		});
	}

	std::string_view NetworkedPlayer::getUsername() {
		return _username;
	}
}