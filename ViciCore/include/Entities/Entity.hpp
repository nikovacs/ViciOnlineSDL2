#pragma once
#include "ViciCore/include/Attributes/Attributes.hpp"
#include <nlohmann/json.hpp>
#include <memory>

namespace Entities {
	class Entity {
	public:
		Entity(nlohmann::json* clientW, nlohmann::json* clientR);
		virtual void setPosition(int x, int y);
		int getX();
		int getY();
		int getWidth();
		void setWidth(int w);
		int getHeight();
		void setHeight(int w);
		virtual void setDir(int dir);
		int getDir();
		Attributes& getClientW();
		Attributes& getClientR();
	protected:
		int _x{}, _y{};
		int _dir{};
		int _width{};
		int _height{};
		Attributes _clientW{};
		Attributes _clientR{};
	};
}