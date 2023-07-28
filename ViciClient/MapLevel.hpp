#pragma once

#include "Level.hpp"
#include <string_view>
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "NetworkAsset.hpp"
#include <memory>

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
		void updateFocusLevel();
		void verifyAndUpdateLevelAssets();
		std::pair<int, int> _mapDimensions{};
		std::vector<std::vector<std::string>> _mapLevels{};
		std::unordered_map<std::string, std::unique_ptr<Networking::NetworkAsset<Levels::Level>>> _levelNameAssetMap{};
		std::pair<int, int> _focusLevel{};
	};
}