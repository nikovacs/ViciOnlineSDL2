#include "ClientPlayerJSWrapper.hpp"
#include "ViciClient.hpp"
#include "PlayerInfo.hpp"
#include <iostream>

namespace JS {
	ClientPlayerJSWrapper::ClientPlayerJSWrapper(Entities::ClientPlayer* pl, v8pp::context* ctx) : AbstractPlayerJSWrapper{ ctx, pl }, _player{ pl } {
		setAllowWriteClientW(true);
	}

	std::string ClientPlayerJSWrapper::getUsername() {
		return PlayerInfo::username;
	}

	int ClientPlayerJSWrapper::getX() {
		return _player->getX();
	}

	void ClientPlayerJSWrapper::setX(int x) {
		_player->setPosition(x, _player->getY());
	}

	int ClientPlayerJSWrapper::getY() {
		return _player->getY();
	}

	void ClientPlayerJSWrapper::setY(int y) {
		_player->setPosition(_player->getX(), y);
	}

	int ClientPlayerJSWrapper::getDir() {
		return _player->getDir();
	}

	void ClientPlayerJSWrapper::setDir(int dir) {
		_player->setDir(dir);
	}

	std::string_view ClientPlayerJSWrapper::getAni() {
		return _player->getAni();
	}

	void ClientPlayerJSWrapper::setAni(std::string_view ani) {
		_player->setAniHard(ani);
	}

	int ClientPlayerJSWrapper::getWidth() {
		return _player->getWidth();
	}

	void ClientPlayerJSWrapper::setWidth(int w) {
		_player->setWidth(w);
	}

	int ClientPlayerJSWrapper::getHeight() {
		return _player->getHeight();
	}

	void ClientPlayerJSWrapper::setHeight(int h) {
		_player->setHeight(h);
	}
}