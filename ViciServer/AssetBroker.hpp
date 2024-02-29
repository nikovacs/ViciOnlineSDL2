#pragma once

#include <unordered_map>
#include <string>
#include <enet/enet.h>
#include <nlohmann/json.hpp>
#include "../ViciEngine/AssetTransfer.hpp"
#include "../ViciEngine/base64.hpp"

namespace Networking {
	class AssetBroker : public AssetTransfer {
	public:
		static void sendFile(ENetEvent& event);
		static inline std::string getFile(std::string_view fileName) {
			if (_assetIndex.count(fileName.data()) > 0) {
				std::string path = _assetIndex[fileName.data()];
				return base64::from_base64(readFile(path));
			}
			return "";
		}
	};
}