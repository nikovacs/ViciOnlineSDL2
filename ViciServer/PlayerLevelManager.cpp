#include "PlayerLevelManager.hpp"

namespace Networking {
	std::unordered_map<std::string, std::set<uint32_t>> _playersOnLevel = {};
	std::unordered_map<std::string, std::set<uint32_t>> _playersWatchingLevel = {};
	std::mutex _lvlMngrMtx = {};
	
	void PlayerLevelManager::addToLevel(uint32_t id, std::string_view levelName) {
		std::lock_guard<std::mutex> lock(_lvlMngrMtx);
		if (!_playersOnLevel.contains(levelName.data())) {
			_playersOnLevel.emplace(levelName, std::set<uint32_t>{});
		}
		_playersOnLevel.at(levelName.data()).insert(id);
	}

	void PlayerLevelManager::removeFromLevel(uint32_t id, std::string_view levelName) {
		std::lock_guard<std::mutex> lock(_lvlMngrMtx);
		if (!_playersOnLevel.contains(levelName.data())) return; // consider removing empty set?
		_playersOnLevel.at(levelName.data()).erase(id);
	}

	void PlayerLevelManager::startWatchingLevel(uint32_t id, std::string_view levelName) {
		std::lock_guard<std::mutex> lock(_lvlMngrMtx);
		if (!_playersWatchingLevel.contains(levelName.data())) {
			_playersWatchingLevel.emplace(levelName, std::set<uint32_t>{});
		}
		_playersWatchingLevel.at(levelName.data()).insert(id);
	}

	void PlayerLevelManager::stopWatchingLevel(uint32_t id, std::string_view levelName) {
		std::lock_guard<std::mutex> lock(_lvlMngrMtx);
		if (!_playersWatchingLevel.contains(levelName.data())) return; // consider removing empty set?
		_playersWatchingLevel.at(levelName.data()).erase(id);
	}
}