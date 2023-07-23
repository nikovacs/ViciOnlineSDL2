#include "ClientPlayer.hpp"

namespace Entities {
	void ClientPlayer::setDir(int dir) {
		EntityAnimated::setDir(dir);
		// TODO send packet to server
	}

	void ClientPlayer::setAniHard(std::string_view aniName) {
		EntityAnimated::setAniHard(aniName);
		// TODO send packet to server
	}

	void ClientPlayer::setPosition(int x, int y) {
		EntityAnimated::setPosition(x, y);
		// TODO send packet to server
	}

	int ClientPlayer::getWidth() {
		return _width;
	}

	void ClientPlayer::setWidth(int w) {
		_width = w;
	}

	int ClientPlayer::getHeight() {
		return _height;
	}
	
	void ClientPlayer::setHeight(int h) {
		_height = h;
	}
}