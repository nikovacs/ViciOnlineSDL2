#pragma once

#include "Level.hpp"
#include <SDL2/SDL.h>
#include <string_view>
#include <unordered_map>
#include <memory>
#include "NetworkAsset.hpp"
#include "Texture.hpp"
#include <vector>
#include <unordered_map>

namespace Levels {
	class SingleLevel : Level {
	public:
		SingleLevel(std::string_view name, std::string_view source);
		virtual ~SingleLevel();
		void update() override;
		void render(SDL_Renderer* renderer) override;
		void renderWithOffsets(SDL_Renderer* renderer, int xOffset, int yOffset);
	private:
		std::unordered_map<int, std::unique_ptr<Networking::NetworkAsset<AssetTypes::Texture>>> _layerTilesetMap{};
		//std::vector<std::pair<SDL_Rect, SDL_Rect>> _tilesSrcDest;
		std::vector<std::unordered_map<int, std::vector<SDL_Rect>>> _tileRects{};
		void parseTileLine(std::string const& line);
	};
}

