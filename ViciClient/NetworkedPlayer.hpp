#pragma once

#include "EntityAnimated.hpp"

namespace Entities {
	class NetworkedPlayer : public EntityAnimated {
	public:
		NetworkedPlayer(std::string_view aniName, int x, int y, int direction) : EntityAnimated(aniName, x, y, direction) {};
	};
}