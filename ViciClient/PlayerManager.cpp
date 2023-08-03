#include "PlayerManager.hpp"
#include <SDL2/SDL.h>

namespace Networking {
	std::unordered_map<uint32_t, std::unique_ptr<Entities::NetworkedPlayer>> PlayerManager::_players{};
	std::mutex PlayerManager::_playerMutex{};

	void PlayerManager::update() {
		std::lock_guard<std::mutex> lock(_playerMutex);
		for (auto& player : _players) {
			player.second->update();
		}
	}

	void PlayerManager::render(SDL_Renderer* renderer) {
		std::lock_guard<std::mutex> lock(_playerMutex);
		for (auto& player : _players) {
			player.second->render(renderer);
		}
	}

	void PlayerManager::spawnPlayer(uint32_t id, int posX, int posY, int dir, std::string_view animationName) {
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (_players.contains(id)) return;
		_players.emplace(id, std::make_unique<Entities::NetworkedPlayer>(animationName, posX, posY, dir));
	}

	void PlayerManager::despawnPlayer(uint32_t id) {
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players.erase(id);
	}

	void PlayerManager::updatePlayerPos(uint32_t id, int x, int y) {
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players[id]->setPosition(x, y);
	}

	void PlayerManager::updatePlayerAniHard(uint32_t id, std::string_view animationName) {
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players[id]->setAniHard(animationName);
	}

	void PlayerManager::updatePlayerAniSoft(uint32_t id, std::string_view animationName) {
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players[id]->setAniSoft(animationName);
	}

	void PlayerManager::updatePlayerDir(uint32_t id, int direction) {
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players[id]->setDir(direction);
	}
}