#pragma once

namespace Entities {
	class Entity {
	protected:
		double _x{}, _y{};
	public:
		virtual void setPosition(double x, double y);
	};
}