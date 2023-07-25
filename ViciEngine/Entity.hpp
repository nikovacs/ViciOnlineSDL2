#pragma once

namespace Entities {
	class Entity {
	protected:
		int _x{}, _y{};
	public:
		virtual void setPosition(int x, int y);
		int getX();
		int getY();
		int getWidth();
		void setWidth(int w);
		int getHeight();
		void setHeight(int w);
	protected:
		int _width{};
		int _height{};
	};
}