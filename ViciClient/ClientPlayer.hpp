#pragma once

#include "EntityAnimated.hpp"

namespace Entities {
	class ClientPlayer : public EntityAnimated {
	public:
		ClientPlayer(std::string_view aniName, int x, int y, int direction) : EntityAnimated(aniName, x, y, direction) {};
		void setDir(int dir) override;
		void setAniHard(std::string_view aniName) override;
		void setPosition(int x, int y) override;
		int getWidth();
		void setWidth(int w);
		int getHeight();
		void setHeight(int w);
	private:
		int _width{};
		int _height{};
	};
}