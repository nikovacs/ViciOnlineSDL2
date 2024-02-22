#pragma once

#include "../ViciEngine/Entity.hpp"
#include "../ViciEngine/Attributes.hpp"
#include <string>
#include <mutex>
#include <set>

namespace Entities {
	class ServerPlayer : public Entity {
	public:
		ServerPlayer(std::string_view username, std::string_view playerId, uint32_t id, std::string_view animation, std::string_view _world,
			int dir, int x, int y, float zoom, nlohmann::json* clientW, nlohmann::json* clientR);
		virtual ~ServerPlayer();
		void setLevel(std::string_view level);
		std::string_view getLevel();
		void setCameraZoom(float zoom);
		float getCameraZoom();
		void setAni(std::string_view animation);
		std::string_view getAni();
		void setWorld(std::string_view world);
		std::string_view getWorld();
		void startWatchingLevel(std::string_view lvl);
		const std::set<std::string>& getLevelsWatching();
		void stopWatchingLevel(std::string_view lvl);
		std::string_view getUsername();
	private:
		uint32_t _connectionId{};
		std::string _animation{};
		std::string _world{};
		std::string _level{};
		float _cameraZoom{ 1.0f };
		std::set<std::string> _levelsWatching{};
		std::string _username{};
		std::string _playerId{};
	};
}