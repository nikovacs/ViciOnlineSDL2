#pragma once
#include <enet/enet.h>
#include <atomic>

#ifdef _WIN32
// Windows-specfic requirements for enet
#pragma comment(lib, "WS2_32")
#pragma comment(lib, "winmm")
#endif // _WIN32

namespace Networking {
	class UdpHost {
	public:
		static inline UdpHost* instance;
		UdpHost(bool isServer, int maxConnections, int port, int numChannels);
		virtual ~UdpHost();
		void start();
		void stop();
		ENetHost* getHost();
	protected:
		virtual void doNetworkLoop(ENetHost* host) = 0;
		std::atomic_bool _isRunning;
		ENetHost* _host;
		ENetAddress _address;
	};
}