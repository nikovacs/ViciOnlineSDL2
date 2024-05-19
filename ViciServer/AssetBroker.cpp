#include "AssetBroker.hpp"
#include "UdpServer.hpp"
#include <string>
#include <nlohmann/json.hpp>
#include "../ViciEngine/base64.hpp"

namespace fs = std::filesystem;

void Networking::AssetBroker::sendFile(ENetPeer* peer, UdpChannels channel, SimplePacket& packet) {
	std::string fileName{ packet.get<std::string>() };
	if (_assetIndex.count(fileName.data()) > 0) {
		std::string path = _assetIndex[fileName.data()];
		std::string fileData = readFile(path);
		if (fileData.empty()) {
			return;
		}

		SimplePacket packet{};
		packet.add<std::string>(fileName);
		packet.add<std::string>(path);
		packet.add<std::string>(fileData);
		std::cout << "sending " << fileName << std::endl;

		Networking::UdpServer::sendSimplePacket(peer, packet, channel, ENET_PACKET_FLAG_RELIABLE);
	}
}

