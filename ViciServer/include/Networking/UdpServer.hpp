#pragma once

#include "header_wrappers/enet_wrapper.h"
#include <string>
#include "ViciCore/include/Networking/UdpHost.hpp"
#include "ViciCore/include/Networking/UdpChannels.hpp"
#include "ViciCore/include/Networking/SimplePacket.hpp"

namespace Networking {
	class UdpServer : public UdpHost {
	public:
		UdpServer(int port, int maxPlayers);
		virtual ~UdpServer();
		void doNetworkLoop(ENetHost* server) override;
		static void sendEmpty(ENetPeer* peer, Networking::UdpChannels channel, ENetPacketFlag flag);
		static void sendJson(ENetPeer* peer, nlohmann::json& json, Networking::UdpChannels channel, ENetPacketFlag flag);
		static void sendSimplePacket(ENetPeer* peer, SimplePacket& packet, Networking::UdpChannels channel, ENetPacketFlag flag);
	};
}