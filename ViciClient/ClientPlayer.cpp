#include "ClientPlayer.hpp"

void Entities::ClientPlayer::setDir(int dir) {
	EntityAnimated::setDir(dir);
	// TODO send packet to server
}

void Entities::ClientPlayer::setAniHard(std::string_view aniName) {
	EntityAnimated::setAniHard(aniName);
	// TODO send packet to server
}

void Entities::ClientPlayer::setPosition(double x, double y) {
	EntityAnimated::setPosition(x, y);
	// TODO send packet to server
}