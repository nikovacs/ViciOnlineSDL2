#include "NetworkedPlayerJSWrapper.hpp"
#include <iostream>

namespace JS {
	NetworkedPlayerJSWrapper::NetworkedPlayerJSWrapper() : _player{nullptr} {}

	NetworkedPlayerJSWrapper::NetworkedPlayerJSWrapper(Entities::NetworkedPlayer* pl) : _player{pl} {}

	NetworkedPlayerJSWrapper::NetworkedPlayerJSWrapper(const NetworkedPlayerJSWrapper& other) : _player{other._player} {}

	std::string_view NetworkedPlayerJSWrapper::getUsername() {
		if (_player)
			return _player->getUsername();
		return "";
	}
	int NetworkedPlayerJSWrapper::getX() {
		if (_player)
			return _player->getX();
		return -1;
	}
	int NetworkedPlayerJSWrapper::getY() {
		if (_player)
			return _player->getY();
		return -1;
	}
	int NetworkedPlayerJSWrapper::getDir() {
		if (_player)
			return _player->getDir();
		return -1;
	}
	std::string_view NetworkedPlayerJSWrapper::getAni() {
		if (_player)
			return _player->getAni();
		return "";
	}
	int NetworkedPlayerJSWrapper::getWidth() {
		if (_player)
			return _player->getWidth();
		return -1;
	}
	int NetworkedPlayerJSWrapper::getHeight() {
		if (_player)
			return _player->getHeight();
		return -1;
	}
}