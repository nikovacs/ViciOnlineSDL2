#pragma once

#include "EntityAnimated.hpp"

namespace Entities {
	class ClientPlayer : public EntityAnimated {
	public:
		ClientPlayer(std::string_view aniName, double x, double y, int direction) : EntityAnimated(aniName, x, y, direction) {};
		void setDir(int dir) override;
		void setAniHard(std::string_view aniName) override;
		void setPosition(double x, double y) override;
	};
}