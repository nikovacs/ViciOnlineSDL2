#include "texture.hpp"
#include "ViciClient.hpp"
#include <SDL2/SDL_image.h>
#include <string>

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