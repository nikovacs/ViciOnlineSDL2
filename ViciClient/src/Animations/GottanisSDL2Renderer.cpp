#include "ViciClient/include/Animations/GottanisSDL2Renderer.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

namespace Animations {

using namespace gottanis;

GottanisSDL2Renderer::GottanisSDL2Renderer(SDL_Renderer *renderer) : _renderer(renderer) {}

void GottanisSDL2Renderer::clearRect(int x, int y, int w, int h, TextureHandle texture) {
    SDL_Texture *atlasTexture = (SDL_Texture *)texture;
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderTarget(_renderer, atlasTexture);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(_renderer, &rect);
    SDL_SetRenderTarget(_renderer, nullptr);
}

void GottanisSDL2Renderer::mergeTexture(int x, int y, int w, int h, TextureHandle _childTexture, TextureHandle _atlasTexture) {
    SDL_Texture *childTexture = (SDL_Texture *)_childTexture;
    SDL_SetTextureBlendMode(childTexture, SDL_BLENDMODE_NONE);

    SDL_Texture *atlasTexture = (SDL_Texture *)_atlasTexture;
    SDL_Rect rect = {x, y, w, h};

    SDL_SetTextureBlendMode(atlasTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(_renderer, atlasTexture);
    SDL_Rect atlasDestRect1 = {x, y, w, h};
    SDL_RenderCopy(_renderer, childTexture, NULL, &atlasDestRect1);
    SDL_SetRenderTarget(_renderer, nullptr);
}

void GottanisSDL2Renderer::getTextureSize(TextureHandle texture, int *w, int *h) {
    SDL_Texture *tex = (SDL_Texture *)texture;
    SDL_QueryTexture(tex, nullptr, nullptr, w, h);
}

TextureHandle GottanisSDL2Renderer::createEmptyTexture(int width, int height) {
    SDL_Texture *texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetTextureBlendMode((SDL_Texture *)texture, SDL_BLENDMODE_BLEND);
    return (TextureHandle)texture;
}
TextureHandle GottanisSDL2Renderer::loadTexture(std::string_view path) {
    // return (TextureHandle)nullptr;
    SDL_Surface *surface = IMG_Load(path.data());
    if (surface == nullptr) {
        std::cerr << "Failed to load image: " << path << std::endl;
        return 0;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
    if (texture == nullptr) {
        std::cerr << "Failed to create texture from surface: " << path << std::endl;
        return 0;
    }

    SDL_FreeSurface(surface);
    return (TextureHandle)texture;
}

void GottanisSDL2Renderer::unloadTexture(TextureHandle texture) {
    SDL_DestroyTexture((SDL_Texture *)texture);
}

void GottanisSDL2Renderer::render(TextureHandle texture, Rect *src, RectF *dest, float rotation, PointF *pivot, RendererFlip flip) {
    if (texture == (TextureHandle) nullptr) {
        return;
    }
    SDL_Rect srcRect;
    SDL_FRect destRect;
    SDL_FPoint pivotPoint;
    if (src != nullptr) {
        srcRect = {src->x, src->y, src->w, src->h};
    }
    if (dest != nullptr) {
        destRect = {dest->x, dest->y, dest->w, dest->h};
    }
    if (pivot != nullptr) {
        pivotPoint = {pivot->x, pivot->y};
    }

    SDL_Rect *srcRectPtr = src == nullptr ? nullptr : &srcRect;
    SDL_FRect *destRectPtr = dest == nullptr ? nullptr : &destRect;
    SDL_FPoint *pivotPointPtr = pivot == nullptr ? nullptr : &pivotPoint;

    SDL_RenderCopyExF(_renderer, (SDL_Texture *)texture, srcRectPtr, destRectPtr, rotation, pivotPointPtr, (SDL_RendererFlip)flip);
}

TextureHandle GottanisSDL2Renderer::createRenderTarget(int width, int height) {
    SDL_Texture *texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetTextureBlendMode((SDL_Texture *)texture, SDL_BLENDMODE_BLEND);
    return (TextureHandle)texture;
}

} // namespace Animations