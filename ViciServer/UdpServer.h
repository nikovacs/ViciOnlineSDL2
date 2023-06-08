#pragma once

#include "../ViciEngine/UdpHost.h"
#include <string>

namespace Networking {
	class UdpServer : public UdpHost {
	public:
		UdpServer(int port, int maxPlayers);
		virtual ~UdpServer();
		void doNetworkLoop(ENetHost* server) override;
	};
}