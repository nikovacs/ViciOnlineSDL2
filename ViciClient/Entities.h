#pragma once

namespace Entities {
	class Entity {
	protected:
		double _x, _y;
	public:
		void setPosition(double x, double y);
	};

	class EntityDrawable : public Entity {
	public:
		virtual void render() = 0;
	};
}