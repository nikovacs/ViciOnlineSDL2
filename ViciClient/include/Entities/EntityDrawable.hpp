#pragma once

#include <SDL2/SDL.h>
#include "ViciCore/include/Entities/Entity.hpp"

namespace Entities {
	class EntityDrawable : public Entity {
	public:
		EntityDrawable(nlohmann::json* clientW, nlohmann::json* clientR);
		virtual void render(SDL_Renderer* renderer) = 0;
	};
}