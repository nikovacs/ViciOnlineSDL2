#include "AssetManager.hpp"
#include "UdpClient.hpp"
#include "../ViciEngine/UdpChannels.hpp"
#include "../ViciEngine/base64.hpp"
#include "UdpChannelMap.hpp"
#include <enet/enet.h>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <fstream>
#include <memory>

#include <iostream>

namespace fs = std::filesystem;

std::unordered_map<std::string, std::weak_ptr<void>> AssetManager::_assetCache = std::unordered_map<std::string, std::weak_ptr<void>>();
std::unordered_map<std::string, std::string> AssetManager::_assetIndex = std::unordered_map<std::string, std::string>();
std::unordered_map<std::string, std::shared_ptr<void>> AssetManager::_assetsInProgress = std::unordered_map<std::string, std::shared_ptr<void>>();

void AssetManager::initializeIndex() {
	// iterate over all files in the assets folder
	// for each file, add it to the assetIndex
	createStructure(); // refactor this to create the structure for any file it is trying to save
	for (const auto& entry : fs::recursive_directory_iterator("Assets")) {
		const auto& path = entry.path();
		if (fs::is_regular_file(path)) {
			_assetIndex.insert(std::make_pair(path.filename().string(), path.string())); // make key always lowercase?
		}
	}
}

void AssetManager::createStructure() {
	// create the assets folder if it does not exist
	fs::create_directories("Assets/ViciDefaults");
	// TODO: Create the directory where the name is the name of the server
}

void AssetManager::requestFileAsBytes(std::string_view fileName, int channelID) {
	std::cout << "requesting " << fileName << " on channel " << channelID << '\n';
	const uint8_t* buffer = reinterpret_cast<const uint8_t*>(fileName.data());
	size_t bufferSize = fileName.length();

	// Create an ENetPacket with the byte buffer
	ENetPacket* packet = enet_packet_create(buffer, bufferSize, ENET_PACKET_FLAG_RELIABLE);

	// Send the packet to the destination peer on the desired channel
	enet_peer_send(static_cast<Networking::UdpClient*>(Networking::UdpClient::instance)->getGameServer(), channelID, packet);
	
	// Flush the client
	enet_host_flush(Networking::UdpClient::instance->getHost());

	// Clean up the packet (ENet will handle the destruction internally after sending)
	enet_packet_destroy(packet);
}

void AssetManager::onBytesReceived(ENetEvent& event) {
	auto jsonString = std::string(reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength);
	nlohmann::json json{ nlohmann::json::parse(jsonString) };

	std::string fileName = json["fileName"];
	std::string path = json["path"];
	std::string fileData = json["data"];

	base64::write_file(path, fileData);
	_assetIndex.emplace(fileName, path);

	//load from path
	std::string_view typeName{ Networking::UdpTypeChannelMap::getTypeFromChannel(event.channelID) };
	std::shared_ptr<void>& assetInProgress = _assetsInProgress.at(fileName);
	if (typeName == "Texture") {
		assetInProgress = std::make_shared<AssetTypes::Texture>(path);
	}

	// put it into the cache
	_assetCache.emplace(fileName, std::weak_ptr(_assetsInProgress.at(fileName)));
}