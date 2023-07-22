#pragma once

#include <SDL2/SDL.h>
#include "Level.hpp"

namespace Client {
	class Camera {
	public:
		Camera();
		virtual ~Camera();
		void update(Levels::Level& level);
		
		
	private:
		SDL_Rect _camera{};
	};
}