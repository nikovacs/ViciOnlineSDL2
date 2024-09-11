#pragma once

#include <SDL2/SDL.h>
#include <string_view>
#include <memory>
#include <nlohmann/json.hpp>
#include "ViciClient/include/Entities/EntityDrawable.hpp"
#include "ViciClient/include/Networking/NetworkAsset.hpp"
#include "ViciClient/include/AssetTypes/Texture.hpp"

namespace Entities {
	class EntityTexture : public EntityDrawable {
	public:
		EntityTexture(std::string_view textureFileName, int x, int y, int w, int h, nlohmann::json* clientW, nlohmann::json* clientR);
		void render(SDL_Renderer* renderer) override;
	protected:
		std::unique_ptr<Networking::NetworkAsset<AssetTypes::Texture>> _texture{ nullptr };
		int _imX, _imY, _imW, _imH;
	};
}