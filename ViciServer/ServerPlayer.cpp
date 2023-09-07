#include "ServerPlayer.hpp"

namespace Entities {
	ServerPlayer::ServerPlayer(uint32_t id, std::string_view animation, std::string_view level, int dir, int x, int y) : _connectionId{ id }, _animation{ animation }, _level{ level } {
		setPosition(x, y);
		_dir = dir;
	}

	ServerPlayer::~ServerPlayer() {}

	void ServerPlayer::setLevel(std::string_view level) {
		_level = level;
	}

	std::string_view ServerPlayer::getLevel() {
		return _level;
	}
	
	void ServerPlayer::setAni(std::string_view animation) {
		_animation = animation;
	}

	std::string_view ServerPlayer::getAni() {
		return _animation;
	}

	void ServerPlayer::setWorld(std::string_view world) {
		_world = world;
	}

	std::string_view ServerPlayer::getWorld() {
		return _world;
	}

	void ServerPlayer::startWatchingLevel(std::string_view lvl) {
		_levelsWatching.insert(lvl.data());
	}

	const std::set<std::string>& ServerPlayer::getLevelsWatching() {
		return _levelsWatching;
	}

	void ServerPlayer::stopWatchingLevel(std::string_view lvl) {
		_levelsWatching.erase(lvl.data());
	}
}