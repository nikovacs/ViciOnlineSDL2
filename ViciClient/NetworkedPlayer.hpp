#pragma once

#include "EntityAnimated.hpp"

namespace Entities {
	class NetworkedPlayer : public EntityAnimated {
	public:
		NetworkedPlayer(std::string_view username, std::string_view aniName, int x, int y, int direction);
		std::string_view getUsername();
	private:
		std::string _username{};
	};
}