#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace Textures {
	SDL_Texture* loadTexture(const char* fileName);
}