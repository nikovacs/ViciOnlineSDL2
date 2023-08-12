#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <set>
#include <mutex>

namespace Networking {
	class PlayerLevelManager {
	public:
		static void addToLevel(uint32_t id, std::string_view levelName);
		static void removeFromLevel(uint32_t id, std::string_view levelName);
		static void startWatchingLevel(uint32_t id, std::string_view levelName);
		static void stopWatchingLevel(uint32_t id, std::string_view levelName);
		static inline std::vector<uint32_t> getPlayersOnLevel(std::string_view levelName) {
			std::lock_guard<std::mutex> lock(_lvlMngrMtx);
			if (!_playersOnLevel.contains(levelName.data())) return {};
			auto playerSet = _playersOnLevel.at(levelName.data());
			return { playerSet.begin(), playerSet.end() };
		}
		static inline std::vector<uint32_t> getPlayersWatchingLevel(std::string_view levelName) {
			std::lock_guard<std::mutex> lock(_lvlMngrMtx);
			if (!_playersWatchingLevel.contains(levelName.data())) return {};
			auto playerSet = _playersWatchingLevel.at(levelName.data());
			return { playerSet.begin(), playerSet.end() };
		}
	private:
		static std::unordered_map<std::string, std::set<uint32_t>> _playersOnLevel;
		static std::unordered_map<std::string, std::set<uint32_t>> _playersWatchingLevel;
		static std::mutex _lvlMngrMtx;
	};
}