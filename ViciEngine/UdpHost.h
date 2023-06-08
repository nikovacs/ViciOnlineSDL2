#pragma once
#include <enet/enet.h>
#include <atomic>

namespace Networking {
	class UdpHost {
	public:
		UdpHost(int maxConnections, int port, int numChannels);
		virtual ~UdpHost();
		void static start();
		void static stop();
	private:
		void static hostLoop();
		static inline std::atomic_bool _isRunning;
		static inline ENetHost* _host;
		static inline ENetAddress _address;
	};
}