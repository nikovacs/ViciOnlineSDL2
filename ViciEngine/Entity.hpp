#pragma once

namespace Entities {
	class Entity {
	public:
		virtual void setPosition(int x, int y);
		int getX();
		int getY();
		int getWidth();
		void setWidth(int w);
		int getHeight();
		void setHeight(int w);
		virtual void setDir(int dir);
		int getDir();
	protected:
		int _x{}, _y{};
		int _dir{};
		int _width{};
		int _height{};
	};
}