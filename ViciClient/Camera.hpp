#pragma once

#include <SDL2/SDL.h>
#include "Level.hpp"
#include "ClientPlayer.hpp"

namespace Client {
	class Camera {
	public:
		Camera();
		virtual ~Camera();
		void initialize();
		void update(Levels::Level& level, Entities::ClientPlayer& player);
		int getX();
		int getY();
		int getW();
		int getH();
		void setScale(float scale);
		float getScale();
		int getOffsetX();
		int getOffsetY();
		
	private:
		SDL_Rect _camera{};
		float _scale{};
		int _offsetX{};
		int _offsetY{};
	};
}