#pragma once

#include <SDL2/SDL.h>
#include "Level.hpp"
#include "../ViciEngine/Entity.hpp"

namespace Client {
	class Camera {
	public:
		Camera();
		virtual ~Camera();
		void initialize();
		void update(Levels::Level& level);
		int getX();
		int getY();
		int getW();
		int getH();
		void setScale(float scale);
		float getScale();
		int getOffsetX();
		int getOffsetY();
		void setFocusObject(Entities::Entity* target);
		inline std::pair<int, int> getFocusPoint() { return std::make_pair(_focusX, _focusY); }
		
	private:
		SDL_Rect _camera{};
		float _scale{};
		int _offsetX{};
		int _offsetY{};
		int _focusX{};
		int _focusY{};
		Entities::Entity* _target{ nullptr };
	};
}