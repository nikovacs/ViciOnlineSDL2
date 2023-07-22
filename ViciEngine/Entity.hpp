#pragma once

namespace Entities {
	class Entity {
	protected:
		int _x{}, _y{};
	public:
		virtual void setPosition(int x, int y);
		int getX() { return _x; };
		int getY() { return _y; };
	};
}