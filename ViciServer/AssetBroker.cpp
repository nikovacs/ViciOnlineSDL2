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

namespace fs = std::filesystem;

std::unordered_map<std::string, std::string> AssetBroker::assetIndex = std::unordered_map<std::string, std::string>();

void AssetBroker::initializeIndex() {
	// iterate over all files in the assets folder
	// for each file, add it to the assetIndex
	createStructure();
	for (const auto& entry : fs::recursive_directory_iterator("Assets")) {
		const auto& path = entry.path();
		if (fs::is_regular_file(path)) {
			assetIndex.insert(std::make_pair(path.filename().string(), path.string())); // make key always lowercase?
		}
	}
	
	// TODO delete
	for (const auto& pair : assetIndex) {
		std::cout << pair.first << " " << pair.second << '\n';
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
		std::string fileData = readFile(path);

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

std::string AssetBroker::readFile(std::string_view filePath) noexcept {
	std::ifstream file(filePath.data(), std::ios::binary);
	
	if (!file) return "";

	std::string fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	return fileData;
}

