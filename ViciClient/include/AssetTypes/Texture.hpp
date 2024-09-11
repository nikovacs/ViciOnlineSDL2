#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace AssetTypes {
	class Texture {
	public:
		Texture(std::string_view fileName);
		~Texture();
		SDL_Texture* getUnderlyingTexture();
	private:
		SDL_Texture* _texture{ nullptr };
	};
}