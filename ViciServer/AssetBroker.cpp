#include "AssetBroker.hpp"
#include "UdpServer.hpp"
#include <string>
#include <enet/enet.h>
#include <nlohmann/json.hpp>

#include <iostream>

namespace fs = std::filesystem;

void Networking::AssetBroker::sendFile(ENetEvent& event) {
	auto fileName = std::string(reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength);
	if (_assetIndex.count(fileName.data()) > 0) {
		std::string path = _assetIndex[fileName.data()];
		std::string fileData = readFile(path);
		if (fileData.empty()) {
			return;
		}
		
		nlohmann::json json;
		json["fileName"] = fileName;
		json["path"] = path;
		json["data"] = fileData;
		
		send(json, event);
	}
}

void Networking::AssetBroker::send(nlohmann::json& json, ENetEvent& event) {
	std::string jsonString{ json.dump() };
	ENetPacket* packet = enet_packet_create(jsonString.c_str(), jsonString.length() + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(event.peer, event.channelID, packet);
	enet_host_flush(Networking::UdpServer::instance->getHost());
}

