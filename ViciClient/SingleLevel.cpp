#include "SingleLevel.hpp"
#include <SDL2/SDL.h>
#include <string_view>
#include <string>
#include <istream>
#include <boost/algorithm/string.hpp>
#include <SDL2/SDL.h>

namespace Levels {
	SingleLevel::SingleLevel(std::string_view name, std::string_view source) : _name{name} {
		std::istringstream iss{ source.data() };
		std::string line;

		std::getline(iss, line);
		_levelDimensions.first = b64ToInt(line.substr(0, 2));
		_levelDimensions.second = b64ToInt(line.substr(2));
		std::getline(iss, line);
		_tileSize = b64ToInt(line);
		std::getline(iss, line);
		bool isTiles{ false };
		int layerIndex{ -1 };
		while (std::getline(iss, line)) {
			if (isTiles) {
				parseTileLine(line);
			}
			else if (line.starts_with("sl")) {
				layerIndex++;
				std::getline(iss, line);
				_layerTilesetMap[layerIndex] = std::make_unique<Networking::NetworkAsset<AssetTypes::Texture>>(line);
				_tileRects.emplace_back();
				isTiles = true;
			}
			else if (line.starts_with("el") && !boost::contains(line, "sl")) {
				isTiles = false;
			}
		}
	}

	SingleLevel::~SingleLevel() {

	}

	void SingleLevel::update() {

	}

	void SingleLevel::render(SDL_Renderer* renderer) {

	}

	void SingleLevel::parseTileLine(std::string const& line) {
		int colonIndex{ static_cast<int>(line.find(":")) };
		int rowIndex{ std::stoi(line.substr(0, colonIndex)) };
		
		auto& map = _tileRects.back();
		map.insert(std::make_pair(rowIndex, std::vector<SDL_Rect>{}));
		auto& vec = map[rowIndex];
		
		for (int i{ 0 }; i < _levelDimensions.first; i += 2) {
			std::string b64Chars{ line.substr(colonIndex + 1 + i, colonIndex + 3 + i) };
			
			if (b64Chars == "--")
				vec.emplace_back();
			
			int tileIndex{ b64ToInt(b64Chars) };
			int tileX{ tileIndex % 128 * _tileSize };
			int tileY{ tileIndex / 128 * _tileSize };
			vec.emplace_back(tileX, tileY, _tileSize, _tileSize);	
		}
	}
}