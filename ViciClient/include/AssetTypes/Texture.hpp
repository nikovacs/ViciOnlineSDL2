#pragma once

#include <string>
#include "header_wrappers/sdl_wrapper.h"

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