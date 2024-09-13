#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include "ViciCore/include/Entities/Entity.hpp"

namespace Entities {
	class EntityDrawable : public Entity {
	public:
		EntityDrawable(std::unique_ptr<nlohmann::json> clientW, std::unique_ptr<nlohmann::json> clientR);
		virtual void render(SDL_Renderer* renderer) = 0;
	};
}