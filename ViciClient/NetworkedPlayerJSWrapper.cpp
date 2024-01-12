#include "NetworkedPlayerJSWrapper.hpp"
#include <v8pp/context.hpp>
#include <v8pp/class.hpp>
#include <iostream>

using namespace v8;

namespace JS {
	NetworkedPlayerJSWrapper::NetworkedPlayerJSWrapper(Entities::NetworkedPlayer* pl, v8pp::context* ctx) : AbstractPlayerJSWrapper{ctx, pl}, _player { pl } {
		setAllowWriteClientW(false);
	}

	std::string NetworkedPlayerJSWrapper::getUsername() {
		return std::string(_player->getUsername());
	}
	int NetworkedPlayerJSWrapper::getX() {
		return _player->getX();
	}
	int NetworkedPlayerJSWrapper::getY() {
		return _player->getY();
	}
	int NetworkedPlayerJSWrapper::getDir() {
		return _player->getDir();
	}
	std::string_view NetworkedPlayerJSWrapper::getAni() {
		return _player->getAni();
	}
	int NetworkedPlayerJSWrapper::getWidth() {
		return _player->getWidth();
	}
	int NetworkedPlayerJSWrapper::getHeight() {
		return _player->getHeight();
	}
}