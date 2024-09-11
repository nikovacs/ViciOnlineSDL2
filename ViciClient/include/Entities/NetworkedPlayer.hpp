#pragma once

#include "ViciClient/include/Entities/EntityAnimated.hpp"

namespace Entities {
	class NetworkedPlayer : public EntityAnimated {
	public:
		NetworkedPlayer(std::string_view username, std::string_view aniName, int x, int y, int direction, nlohmann::json* clientW, nlohmann::json* clientR);
		std::string_view getUsername();
	private:
		std::string _username{};
	};
}