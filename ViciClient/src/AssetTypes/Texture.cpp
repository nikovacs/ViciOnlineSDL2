#include "header_wrappers/sdl_wrapper.h"
#include <string>
#include "ViciClient/include/ViciClient.hpp"
#include "ViciClient/include/AssetTypes/Texture.hpp"

AssetTypes::Texture::Texture(std::string_view source) {
	SDL_RWops* rw = SDL_RWFromConstMem(source.data(), static_cast<int>(source.size()));
	_texture = IMG_LoadTexture_RW(ViciClient::instance->getRenderer(), rw, 0);
	SDL_RWclose(rw);
}

AssetTypes::Texture::~Texture() {
	if (_texture)
		SDL_DestroyTexture(_texture);
}

SDL_Texture* AssetTypes::Texture::getUnderlyingTexture() {
	return _texture;
}