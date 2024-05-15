#pragma once

#include "../ViciEngine/UdpHost.hpp"
#include "../ViciEngine/UdpChannels.hpp"
#include "../ViciEngine/SimplePacket.hpp"
#include <enet/enet.h>
#include <string>

namespace Networking {
	class UdpServer : public UdpHost {
	public:
		UdpServer(int port, int maxPlayers);
		virtual ~UdpServer();
		void doNetworkLoop(ENetHost* server) override;
		static void sendJson(ENetPeer* peer, nlohmann::json& json, Networking::UdpChannels channel, ENetPacketFlag flag);
		static void sendSimplePacket(ENetPeer* peer, SimplePacket& packet, Networking::UdpChannels channel, ENetPacketFlag flag);
	};
}