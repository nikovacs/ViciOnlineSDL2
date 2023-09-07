#include "ClientPlayerManager.hpp"
#include <SDL2/SDL.h>

namespace Networking {
	std::unordered_map<uint32_t, std::unique_ptr<Entities::NetworkedPlayer>> ClientPlayerManager::_players{};
	std::mutex ClientPlayerManager::_playerMutex{};

	void ClientPlayerManager::update() {
		std::lock_guard<std::mutex> lock(_playerMutex);
		for (auto& player : _players) {
			player.second->update();
		}
	}

	void ClientPlayerManager::render(SDL_Renderer* renderer) {
		std::lock_guard<std::mutex> lock(_playerMutex);
		for (auto& player : _players) {
			player.second->render(renderer);
		}
	}

	void ClientPlayerManager::spawnPlayer(nlohmann::json& json) {
		uint32_t id = json["id"];
		int x = json["x"];
		int y = json["y"];
		int w = json["w"];
		int h = json["h"];
		int dir = json["dir"];
		std::string animation = json["animation"];
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (_players.contains(id)) return;
		_players.emplace(id, std::make_unique<Entities::NetworkedPlayer>(animation, x, y, dir));
		_players.at(id)->setHeight(h);
		_players.at(id)->setWidth(w);
	}

	void ClientPlayerManager::despawnPlayer(nlohmann::json& json) {
		uint32_t id = json["id"];
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players.erase(id);
	}

	void ClientPlayerManager::updatePlayerPos(nlohmann::json& json) {
		uint32_t id = json["id"];
		int x = json["x"];
		int y = json["y"];
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players[id]->setPosition(x, y);
	}

	void ClientPlayerManager::updatePlayerAni(nlohmann::json& json) {
		uint32_t id = json["id"];
		std::string animation = json["ani"];
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players[id]->setAniHard(animation);
	}

	void ClientPlayerManager::updatePlayerDir(nlohmann::json& json) {
		uint32_t id = json["id"];
		int dir = json["dir"];
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players[id]->setDir(dir);
	}
}