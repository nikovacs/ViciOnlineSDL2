#include <iostream>
#include "ViciClient/include/ViciClient.hpp"
#include "ViciClient/include/PlayerInfo.hpp"
#include "ViciClient/include/JSRuntime/ClientPlayerJSWrapper.hpp"

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

	std::string ClientPlayerJSWrapper::getDir() {
		return _player->getSkeleton()->getDirection();
	}

	void ClientPlayerJSWrapper::setDir(std::string dir) {
		_player->getSkeleton()->setDirection(dir);
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