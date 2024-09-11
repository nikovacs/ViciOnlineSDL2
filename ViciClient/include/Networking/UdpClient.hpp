#pragma once

#include <enet/enet.h>
#include <string>
#include <nlohmann/json.hpp>
#include "ViciCore/include/Networking/UdpHost.hpp"
#include "ViciCore/include/Networking/UdpChannels.hpp"
#include "ViciCore/include/Networking/SimplePacket.hpp"

namespace Networking {
	class UdpClient : public UdpHost {
	public:
		UdpClient(const std::string_view url, int port);
		virtual ~UdpClient();
		void doNetworkLoop(ENetHost* client) override;
		ENetPeer* getGameServer();
		static void sendJson(nlohmann::json& json, Networking::UdpChannels channel, ENetPacketFlag flag);
		static void sendSimplePacket(SimplePacket& simplePacket, Networking::UdpChannels channel, ENetPacketFlag flag);
	private:
		ENetPeer* _gameServer;
	};
}