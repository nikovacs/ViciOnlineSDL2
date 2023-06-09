#pragma once

#include <SDL2/SDL.h>
#include "../ViciEngine/Entity.h"

namespace Entities {
	class EntityDrawable : public Entity {
	public:
		virtual void render(SDL_Renderer* renderer) = 0;
	};
}