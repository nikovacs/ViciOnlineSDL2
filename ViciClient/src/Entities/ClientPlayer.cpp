#include <nlohmann/json.hpp>
#include <enet/enet.h>
#include <iostream>
#include "ViciCore/include/Networking/SimplePacket.hpp"
#include "ViciClient/include/Entities/ClientPlayer.hpp"
#include "ViciClient/include/Networking/UdpClient.hpp"

namespace Entities {
	ClientPlayer::ClientPlayer(nlohmann::json& json) : EntityAnimated(json["animation"].get<std::string>(), json["x"].get<int>(), json["y"].get<int>(), json["dir"].get<int>(), new nlohmann::json(json["clientW"]), new nlohmann::json(json["clientR"])) {
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
		Networking::SimplePacket packet{};
		packet.add(_dir);
		Networking::UdpClient::sendSimplePacket(packet, Networking::UdpChannels::UpdatePlayerDir, ENET_PACKET_FLAG_RELIABLE);
	}

	void ClientPlayer::setAniHard(std::string_view aniName) {
		EntityAnimated::setAniHard(aniName);

		Networking::SimplePacket packet{};
		packet.add<std::string>(aniName.data());
		Networking::UdpClient::sendSimplePacket(packet, Networking::UdpChannels::UpdatePlayerAni, ENET_PACKET_FLAG_RELIABLE);
	}

	void ClientPlayer::setPosition(int x, int y) {
		EntityAnimated::setPosition(x, y);

		Networking::SimplePacket packet{};
		packet.add(_x);
		packet.add(_y);
		Networking::UdpClient::sendSimplePacket(packet, Networking::UdpChannels::UpdatePlayerPos, ENET_PACKET_FLAG_RELIABLE);
	}
}