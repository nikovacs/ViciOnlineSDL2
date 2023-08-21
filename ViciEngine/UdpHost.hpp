#pragma once
#include <enet/enet.h>
#include <atomic>
#include <nlohmann/json.hpp>
#include <string>
#include "../ViciEngine/base64.hpp"

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
		static inline nlohmann::json getJsonFromPacket(ENetPacket* packet) {
			auto jsonString = std::string(reinterpret_cast<const char*>(packet->data), packet->dataLength);
			return nlohmann::json::parse(jsonString);
		}
		static inline std::string prepareJsonForSending(nlohmann::json json) {
			return json.dump();
		}
	protected:
		virtual void doNetworkLoop(ENetHost* host) = 0;
		std::atomic_bool _isRunning;
		ENetHost* _host;
		ENetAddress _address;
	};
}