#include "AssetManager.h"
#include "UdpClient.h"
#include <enet/enet.h>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <iostream> //TODO remove
#include <nlohmann/json.hpp>
#include <fstream>
#include <memory>

namespace fs = std::filesystem;

//std::unordered_map<std::string, std::weak_ptr<Networking::AbstractNetworkAsset>> AssetManager::_assetsInProgress = std::unordered_map<std::string, std::weak_ptr<Networking::AbstractNetworkAsset>>();

std::unordered_map<std::string, std::weak_ptr<void>> AssetManager::_assetCache = std::unordered_map<std::string, std::weak_ptr<void>>();

std::unordered_map<std::string, std::string> AssetManager::_assetIndex = std::unordered_map<std::string, std::string>();

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

	// TODO delete
	for (const auto& pair : _assetIndex) {
		std::cout << pair.first << " " << pair.second << '\n';
	}
}

void AssetManager::createStructure() {
	// create the assets folder if it does not exist
	fs::create_directories("Assets/ViciDefaults");
	// TODO: Create the directory where the name is the name of the server
}

template <typename T>
void AssetManager::resolve(Networking::NetworkAsset<T>* asset) {
	if (_assetCache.count(asset->getFileName())) {
		if (!_assetCache[asset->getFileName()].expired()) {
			asset.finish((T*)_assetCache[asset->getFileName()].lock());
			return;
		}
		_assetCache.erase(asset->getFileName()); // Right now, just remove from cache once no references are left
	}
}

void AssetManager::writeFile(std::string_view filePath, std::string_view fileData) {
	std::ofstream file(filePath.data(), std::ios::binary);

	if (!file) {
		return;
	}

	file.write(fileData.data(), fileData.length());
	file.close();
}

template <unsigned int N>
void AssetManager::requestFileAsBytes(std::string_view fileName) {
	const uint8_t* buffer = reinterpret_cast<const uint8_t*>(fileName.data());
	size_t bufferSize = fileName.length();

	// Create an ENetPacket with the byte buffer
	ENetPacket* packet = enet_packet_create(buffer, bufferSize, ENET_PACKET_FLAG_RELIABLE);

	// Send the packet to the destination peer on the desired channel
	enet_peer_send(static_cast<Networking::UdpClient*>(Networking::UdpClient::instance)->getGameServer(), N, packet);
	
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

	writeFile(path, fileData);
	_assetIndex.emplace(fileName, path);

	// TODO use the path of the newly saved file to load it into the game
}