#pragma once

#include "EntityDrawable.hpp"
#include "NetworkAsset.hpp"
#include "Texture.hpp"
#include <SDL2/SDL.h>
#include <string_view>
#include <memory>

namespace Entities {
	class EntityTexture : public EntityDrawable {
	public:
		EntityTexture(std::string_view textureFileName, int x, int y, int w, int h);
		void render(SDL_Renderer* renderer) override;
	protected:
		std::unique_ptr<Networking::NetworkAsset<AssetTypes::Texture>> _texture{ nullptr };
		int _imX, _imY, _imW, _imH;
	};
}