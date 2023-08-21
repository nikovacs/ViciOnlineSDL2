#pragma once

#include "../ViciEngine/UdpHost.hpp"
#include "../ViciEngine/UdpChannels.hpp"
#include <enet/enet.h>
#include <string>
#include "nlohmann/json.hpp"

namespace Networking {
	class UdpClient : public UdpHost {
	public:
		UdpClient(const std::string_view url, int port);
		virtual ~UdpClient();
		void doNetworkLoop(ENetHost* client) override;
		ENetPeer* getGameServer();
		static void sendJson(nlohmann::json& json, Networking::UdpChannels channel, ENetPacketFlag flag);
	private:
		ENetPeer* _gameServer;
	};
}