#pragma once

#include "Level.hpp"
#include <string_view>
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "NetworkAsset.hpp"

namespace Levels {
	class MapLevel : Level {
	public:
		MapLevel(std::string_view name, std::string_view source);
		virtual ~MapLevel();
		void update() override;
		void render(SDL_Renderer* renderer) override;
		virtual int getLevelHeight() override;
		virtual int getLevelWidth() override;
	private:
		std::pair<int, int> _mapDimensions{};
		std::vector<std::vector<std::string>> _mapLevels{};
		std::unordered_map<std::string, Networking::NetworkAsset<Levels::Level>> _levelNameAssetMap{};
	};
}