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
		static void sendFile(ENetEvent& event, bool isScript = false);
		static inline std::string getFile(std::string_view fileName) {
			std::string lowercaseFilename{ fileName };
			toLowercase(lowercaseFilename);

			if (containsFile(lowercaseFilename)) {
				std::string path = _assetIndex[lowercaseFilename];
				return base64::from_base64(readFile(path));
			}
			return "";
		}
		static bool containsFile(std::string_view fileName) {
			std::string lowercaseFilename{ fileName };
			toLowercase(lowercaseFilename);
			return _assetIndex.count(lowercaseFilename) > 0;
		}
		inline static void toLowercase(std::string& str) {
			std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		};
	};
}