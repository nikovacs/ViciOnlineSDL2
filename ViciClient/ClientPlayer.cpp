#include "ClientPlayer.hpp"

namespace Entities {
	void ClientPlayer::setDir(int dir) {
		Entity::setDir(dir);
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
}