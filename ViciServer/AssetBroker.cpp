#include "AssetBroker.hpp"
#include "UdpServer.hpp"
#include <string>
#include <enet/enet.h>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

void Networking::AssetBroker::sendFile(ENetEvent& event, bool isScript) {
	auto fileName = std::string(reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength);
	if (isScript) {
		fileName = "cs_" + fileName;
	}
	if (_assetIndex.count(fileName.data()) > 0) {
		std::string path = _assetIndex[fileName.data()];
		std::string fileData = readFile(path);
		if (fileData.empty()) {
			return;
		}

		if (isScript) {
			// remove cs_
			fileName = fileName.substr(3);
		}
		
		nlohmann::json json;
		json["fileName"] = fileName;
		json["path"] = path;
		json["data"] = fileData;
		std::cout << "sending " << fileName << std::endl;
		Networking::UdpServer::sendJson(event.peer, json, static_cast<Networking::UdpChannels>(event.channelID), ENET_PACKET_FLAG_RELIABLE);
	}
}

