#include "PlayerManager.hpp"

namespace Networking {
	std::unordered_map<uint32_t, std::unique_ptr<Entities::ServerPlayer>> PlayerManager::_players{};
	std::mutex PlayerManager::_playerMutex{};

	void PlayerManager::spawnPlayer(uint32_t id) {
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (_players.contains(id)) return;
		_players.emplace(id, std::make_unique<Entities::ServerPlayer>());
		// enet logic
	}
	void PlayerManager::despawnPlayer(uint32_t id) {
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players.erase(id);
		// enet logic
	}
	void PlayerManager::updatePlayerPos(uint32_t id) {
		std::lock_guard<std::mutex> lock(_playerMutex);
	
	}
	void PlayerManager::updatePlayerAniHard(uint32_t id) {
		std::lock_guard<std::mutex> lock(_playerMutex);
	
	}
	void PlayerManager::updatePlayerAniSoft(uint32_t id) {
		std::lock_guard<std::mutex> lock(_playerMutex);
	
	}
	void PlayerManager::updatePlayerDir(uint32_t id) {
		std::lock_guard<std::mutex> lock(_playerMutex);
	
	}
}