#include "PlayerManager.hpp"

namespace Networking {
	std::unordered_map<uint32_t, std::unique_ptr<Entities::ServerPlayer>> PlayerManager::_players{};
	std::mutex PlayerManager::_playerMutex{};

	void PlayerManager::spawnPlayer() {}
	void PlayerManager::despawnPlayer() {}
	void PlayerManager::updatePlayerPos() {}
	void PlayerManager::updatePlayerAniHard() {}
	void PlayerManager::updatePlayerAniSoft() {}
	void PlayerManager::updatePlayerDir() {}
}