#include "NetworkedPlayer.hpp"

namespace Entities {
	NetworkedPlayer::NetworkedPlayer(std::string_view username, std::string_view aniName, int x, int y, int direction) : EntityAnimated(aniName, x, y, direction), _username{username} {}

	std::string_view NetworkedPlayer::getUsername() {
		return _username;
	}
}