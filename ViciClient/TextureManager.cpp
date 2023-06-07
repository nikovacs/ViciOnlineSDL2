#include "TextureManager.h"
#include "ViciClient.h"

SDL_Texture* Textures::loadTexture(const char* fileName) {
	return IMG_LoadTexture(ViciClient::instance->getRenderer(), fileName);
}