#include "AssetBroker.h"
#include "UdpServer.h"
#include <string>
#include <unordered_map>
#include <filesystem>
#include <iostream> // TODO delete
#include <enet/enet.h>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <cctype>
#include "../ViciEngine/base64.h"

namespace fs = std::filesystem;

std::unordered_map<std::string, std::string> AssetBroker::assetIndex = std::unordered_map<std::string, std::string>();

void AssetBroker::initializeIndex() {
	// iterate over all files in the assets folder
	// for each file, add it to the assetIndex
	createStructure();
	for (const auto& entry : fs::recursive_directory_iterator("Assets")) {
		const auto& path = entry.path();
		if (fs::is_regular_file(path)) {
			auto filename_str = path.filename().string();
			std::transform(filename_str.begin(), filename_str.end(), filename_str.begin(),
				[](unsigned char c) { return std::tolower(c); });
			assetIndex.insert(std::make_pair( filename_str, path.string()));
		}
	}
}

void AssetBroker::createStructure() {
	// create the assets folder if it does not exist
	fs::create_directories("Assets/ViciDefaults");
	// TODO: Create the directory where the name is the name of the server
}

void AssetBroker::sendFileAsBytes(ENetEvent& event) {
	auto fileName = std::string(reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength);
	if (assetIndex.count(fileName.data()) > 0) {
		std::string path = assetIndex[fileName.data()];
		std::string fileData = base64::read_file(path);

		if (fileData.empty()) {
			return;
		}
		
		nlohmann::json json;
		json["fileName"] = fileName;
		json["path"] = path;
		json["data"] = fileData;

		std::string jsonString{ json.dump() };

		// Create an ENetPacket with the file data
		ENetPacket* packet = enet_packet_create(jsonString.c_str(), jsonString.length() + 1, ENET_PACKET_FLAG_RELIABLE);

		// Send the packet back to the sender on the same channel
		enet_peer_send(event.peer, event.channelID, packet);
		enet_host_flush(Networking::UdpServer::instance->getHost());
	}
}

