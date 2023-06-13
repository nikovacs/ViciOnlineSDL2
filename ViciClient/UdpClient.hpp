#pragma once

#include "../ViciEngine/UdpHost.hpp"
#include <enet/enet.h>
#include <string>

namespace Networking {
	class UdpClient : public UdpHost {
	public:
		UdpClient(const std::string_view url, int port);
		virtual ~UdpClient();
		void doNetworkLoop(ENetHost* client) override;
		ENetPeer* getGameServer();
	private:
		ENetPeer* _gameServer;
	};
}