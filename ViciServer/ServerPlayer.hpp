#pragma once

#include "../ViciEngine/Entity.hpp"
#include <string>
#include <mutex>
#include <set>

namespace Entities {
	class ServerPlayer : public Entity {
	public:
		ServerPlayer(uint32_t id, std::string_view animation, std::string_view level, int dir, int x, int y);
		virtual ~ServerPlayer();
		void setLevel(std::string_view level);
		std::string_view getLevel();
		void setAniSoft(std::string_view animation);
		void setAniHard(std::string_view animation);
		std::string_view getAni();
	private:
		uint32_t _connectionId{};
		std::string _animation{};
		std::string _level{};
		std::set<std::string> _levelsWatching{};
	};
}