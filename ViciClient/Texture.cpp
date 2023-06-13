#include "texture.hpp"
#include "ViciClient.hpp"
#include <SDL2/SDL_image.h>
#include <string>

AssetTypes::Texture::Texture(std::string_view fileName) {
	_texture = IMG_LoadTexture(ViciClient::instance->getRenderer(), fileName.data());
}

AssetTypes::Texture::~Texture() {
	if (_texture)
		SDL_DestroyTexture(_texture);
}

SDL_Texture* AssetTypes::Texture::getUnderlyingTexture() {
	return _texture;
}