#include "AssetBroker.hpp"
#include "UdpServer.hpp"
#include <string>
#include <enet/enet.h>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

void Networking::AssetBroker::sendFile(ENetEvent& event) {
	auto filePath = std::string(reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength);
	if (indexContains(filePath)) {
		std::string fileData = base64::to_base64(readFile(filePath));
		if (fileData.empty()) {
			return;
		}
		
		nlohmann::json json;
		json["path"] = filePath;
		json["data"] = fileData;
		std::cout << "sending " << filePath << std::endl;
		Networking::UdpServer::sendJson(event.peer, json, static_cast<Networking::UdpChannels>(event.channelID), ENET_PACKET_FLAG_RELIABLE);
	}
}

