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
		/*
		* Sends a file to the client
		* 
		* @param event: The Enet event containing the desired file
		*/
		static void sendFile(ENetEvent& event);

		/*
		* Wrapper around readFile that checks asset index before trying
		* to read from the file system
		* 
		* @param path: The path to the file
		* @return: The file as a string (empty if not found)
		*/
		static inline std::string getFile(std::string_view path) {
			if (indexContains(path)) {
				return readFile(path);
			}
			return "";
		}
	};
}