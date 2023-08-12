#pragma once

#include <unordered_map>
#include <memory>
#include "NetworkedPlayer.hpp"
#include <SDL2/SDL.h>
#include <mutex>

namespace Networking {
	class PlayerManager {
	public:
		PlayerManager() = delete;
		static void update();
		static void render(SDL_Renderer* renderer);

		static void spawnPlayer(nlohmann::json& json);
		static void despawnPlayer(uint32_t id);
		static void updatePlayerPos(uint32_t id, int x, int y);
		static void updatePlayerAniHard(uint32_t id, std::string_view animationName);
		static void updatePlayerAniSoft(uint32_t id, std::string_view animationName); // might not need, server can already see player's current animation
		static void updatePlayerDir(uint32_t id, int direction);
	private:
		static std::unordered_map<uint32_t, std::unique_ptr<Entities::NetworkedPlayer>> _players;
		static std::mutex _playerMutex;
	};
}