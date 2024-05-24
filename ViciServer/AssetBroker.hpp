#pragma once

#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>
#include "../ViciEngine/SimplePacket.hpp"
#include "../ViciEngine/AssetTransfer.hpp"
#include "../ViciEngine/UdpChannels.hpp"
#include <enet/enet.h>

namespace Networking {
	class AssetBroker : public AssetTransfer {
	public:
		static void sendFile(ENetPeer* peer, UdpChannels channel, SimplePacket& packet);
		static inline std::string getFile(std::string_view fileName) {
			std::string lowerFileName = toLower(fileName.data());
			if (_assetIndex.contains(lowerFileName)) {
				std::string path = _assetIndex[lowerFileName];
				return readFile(path);
			}
			return "";
		}
	};
}