#pragma once

#include <string_view>
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "ViciClient/include/Levels/Level.hpp"
#include "ViciClient/include/Networking/NetworkAsset.hpp"

namespace Levels {
	class MapLevel : Level {
	public:
		MapLevel(std::string_view name, std::string_view source);
		virtual ~MapLevel();
		void update() override;
		void render(SDL_Renderer* renderer) override;
		virtual int getLevelHeight() override;
		virtual int getLevelWidth() override;
		virtual void getRenderedLevels(std::set<std::string>& lvlsOut) override;
	private:
		void updateFocusLevel();
		void verifyAndUpdateLevelAssets();
		std::pair<int, int> _mapDimensions{};
		std::vector<std::vector<std::string>> _mapLevels{};
		std::unordered_map<std::string, std::unique_ptr<Networking::NetworkAsset<Levels::Level>>> _levelNameAssetMap{};
		std::pair<int, int> _focusLevel{};
	};
}