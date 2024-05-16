#include "LevelWatchingManager.hpp"
#include "UdpClient.hpp"
#include "nlohmann/json.hpp"
#include "../ViciEngine/UdpChannels.hpp"

namespace Networking {
	void LevelWatchingManager::update(Levels::Level& lvl) {
		std::set<std::string> newLevelsWatching{};
		lvl.getRenderedLevels(newLevelsWatching);
		
		std::set<std::string> startWatchingSet{};
		std::set<std::string> stopWatchingSet{};

		std::set_difference(newLevelsWatching.begin(), newLevelsWatching.end(), _levelsWatching.begin(), _levelsWatching.end(), std::inserter(startWatchingSet, startWatchingSet.begin()));
		std::set_difference(_levelsWatching.begin(), _levelsWatching.end(), newLevelsWatching.begin(), newLevelsWatching.end(), std::inserter(stopWatchingSet, stopWatchingSet.begin()));

		_levelsWatching = newLevelsWatching;

		if (!startWatchingSet.empty()) {
			startWatching(startWatchingSet);
		}
		
		if (!stopWatchingSet.empty()) {
			stopWatching(stopWatchingSet);
		}
	}

	void LevelWatchingManager::startWatching(std::set<std::string>& lvlNames) {
		for (auto& lvlName : lvlNames) {
			SimplePacket packet{};
			packet.add(lvlName);
			UdpClient::sendSimplePacket(packet, Networking::UdpChannels::StartWatchingLevel, ENET_PACKET_FLAG_RELIABLE);
		}
	}
	
	void LevelWatchingManager::stopWatching(std::set<std::string>& lvlNames) {
		for (auto& lvlName : lvlNames) {
			SimplePacket packet{};
			packet.add(lvlName);
			UdpClient::sendSimplePacket(packet, Networking::UdpChannels::StopWatchingLevel, ENET_PACKET_FLAG_RELIABLE);
		}
	}
}