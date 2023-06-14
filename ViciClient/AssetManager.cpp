#include "AssetManager.hpp"
#include "UdpClient.hpp"
#include "../ViciEngine/UdpChannels.hpp"
#include "../ViciEngine/base64.hpp"
#include "../ViciEngine/AssetTransfer.hpp"
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

std::unordered_map<std::string, std::weak_ptr<void>> Networking::AssetManager::_assetCache = std::unordered_map<std::string, std::weak_ptr<void>>();
std::unordered_map<std::string, std::shared_ptr<void>> Networking::AssetManager::_assetsInProgress = std::unordered_map<std::string, std::shared_ptr<void>>();


void Networking::AssetManager::requestFile(std::string_view fileName, int channelID) {
	const uint8_t* buffer = reinterpret_cast<const uint8_t*>(fileName.data());
	size_t bufferSize = fileName.length();

	// Create an ENetPacket with the byte buffer
	ENetPacket* packet = enet_packet_create(buffer, bufferSize, ENET_PACKET_FLAG_RELIABLE);

	// Send the packet to the destination peer on the desired channel
	enet_peer_send(static_cast<Networking::UdpClient*>(Networking::UdpClient::instance)->getGameServer(), channelID, packet);
	
	// Flush the client
	enet_host_flush(Networking::UdpClient::instance->getHost());
}

void Networking::AssetManager::onReceived(ENetEvent& event) {
	auto jsonString = std::string(reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength);
	nlohmann::json json{ nlohmann::json::parse(jsonString) };

	std::string fileName = json["fileName"];
	std::string path = json["path"];
	std::string fileData = json["data"];

	writeFile(path, fileData);
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