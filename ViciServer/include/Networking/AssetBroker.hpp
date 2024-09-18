#pragma once

#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>
#include "header_wrappers/enet_wrapper.h"
#include "ViciCore/include/Networking/SimplePacket.hpp"
#include "ViciCore/include/Networking/AssetTransfer.hpp"
#include "ViciCore/include/Networking/UdpChannels.hpp"

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