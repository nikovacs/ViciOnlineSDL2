#pragma once

#include <set>
#include <string>
#include "ViciClient/include/Levels/Level.hpp"

namespace Networking {
	class LevelWatchingManager {
	public:
		void update(Levels::Level& lvl);
	private:
		std::set<std::string> _levelsWatching{};

		void startWatching(std::set<std::string>& lvlNames);
		void stopWatching(std::set<std::string>& lvlNames);
	};
}