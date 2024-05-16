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
			if (_assetIndex.count(fileName.data()) > 0) {
				std::string path = _assetIndex[fileName.data()];
				return readFile(path);
			}
			return "";
		}
	};
}