#include "ClientPlayerManager.hpp"
#include <SDL2/SDL.h>
#include <iostream>

namespace Networking {
	std::unordered_map<uint32_t, std::unique_ptr<Entities::NetworkedPlayer>> ClientPlayerManager::_players{};
	std::unordered_map<std::string, uint32_t> ClientPlayerManager::_playerUsernamesToIds{};
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
		std::string username = json["usr"];
		int x = json["x"];
		int y = json["y"];
		int w = json["w"];
		int h = json["h"];
		int dir = json["dir"];
		nlohmann::json* clientW = new nlohmann::json(json["clientW"]);
		nlohmann::json* clientR = new nlohmann::json(json["clientR"]);
		std::string animation = json["animation"];
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (_players.contains(id)) return;
		// NetworkedPlayer takes ownership of the clientW and clientR pointers
		_players.emplace(id, std::make_unique<Entities::NetworkedPlayer>(username, animation, x, y, dir, clientW, clientR));
		_players.at(id)->setHeight(h);
		_players.at(id)->setWidth(w);
		_playerUsernamesToIds.emplace(username, id);
	}

	void ClientPlayerManager::despawnPlayer(SimplePacket& packet) {
		uint32_t id = packet.get<uint32_t>();
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_playerUsernamesToIds.erase(_players.at(id)->getUsername().data());
		_players.erase(id);
	}

	void ClientPlayerManager::updatePlayerPos(SimplePacket& packet) {
		int x{ packet.get<int>() };
		int y{ packet.get<int>() };
		uint32_t id{ packet.get<uint32_t>() };

		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players[id]->setPosition(x, y);
	}

	void ClientPlayerManager::updatePlayerAni(SimplePacket& packet) {
		std::string animation{ packet.get<std::string>() };
		uint32_t id = packet.get<uint32_t>();

		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players[id]->setAniHard(animation);
	}

	void ClientPlayerManager::updatePlayerDir(SimplePacket& json) {
		int dir = json.get<int>();
		uint32_t id = json.get<uint32_t>();

		std::cout << "Updating playerid " << id << "to dir " << dir << std::endl;

		std::lock_guard<std::mutex> lock(_playerMutex);
		if (!_players.contains(id)) return;
		_players[id]->setDir(dir);
	}

	Entities::NetworkedPlayer* ClientPlayerManager::getPlayer(std::string_view username) {
		std::lock_guard<std::mutex> lock(_playerMutex);
		if (_playerUsernamesToIds.contains(username.data())) {
			return _players.at(_playerUsernamesToIds.at(username.data())).get();
		}
		return nullptr;
	}
}