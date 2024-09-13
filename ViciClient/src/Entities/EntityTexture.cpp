#include "ViciClient/include/Entities/EntityTexture.hpp"
#include <memory>

Entities::EntityTexture::EntityTexture(std::string_view textureFileName, int im_x, int im_y, int im_w, int im_h,
                                       std::unique_ptr<nlohmann::json> clientW, std::unique_ptr<nlohmann::json> clientR)
    : EntityDrawable(std::move(clientW), std::move(clientR)),
      _texture{std::make_unique<Networking::NetworkAsset<AssetTypes::Texture>>(textureFileName)},
      _imX{im_x}, _imY{im_y}, _imW{im_w}, _imH{im_h} {}

void Entities::EntityTexture::render(SDL_Renderer *renderer) {
    if (!_texture->getValue())
        return;

    SDL_Rect src{_imX, _imY, _imW, _imH};
    SDL_Rect dest{static_cast<int>(_x), static_cast<int>(_y), _imW, _imH};

    SDL_RenderCopy(renderer, _texture->getValue()->getUnderlyingTexture(), &src, &dest);
}