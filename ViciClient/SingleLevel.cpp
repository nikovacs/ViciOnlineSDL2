#include "SingleLevel.hpp"
#include <SDL2/SDL.h>
#include <string_view>
#include <string>
#include <istream>
#include <boost/algorithm/string.hpp>
#include <SDL2/SDL.h>
#include "GameScene.hpp"

namespace Levels {
	SingleLevel::SingleLevel(std::string_view name, std::string_view source) {
		_name = name;
		std::istringstream iss{ source.data() };
		std::string line;

		std::getline(iss, line); // first line (level dimensions)
		boost::trim(line);
		line = line.substr(line.size() - 4); // Necessary if the file begins with BOM denoting utf-8
		_levelDimensions.first = b64ToInt(line.substr(0, 2));
		_levelDimensions.second = b64ToInt(line.substr(2));
		
		std::getline(iss, line); // second line (tile size)
		_tileSize = std::stoi(line);
		
		bool isTiles{ false };
		int layerIndex{ -1 };
		while (std::getline(iss, line)) {
			boost::trim(line);
			if (line.starts_with("sl")) {
				layerIndex++;
				std::getline(iss, line);
				boost::trim(line);
				_layerTilesetMap[layerIndex] = std::make_unique<Networking::NetworkAsset<AssetTypes::Texture>>(line);
				_tileRects.emplace_back();
				isTiles = true;
			}
			else if (line.starts_with("el") && !boost::contains(line, "sl")) {
				isTiles = false;
			}
			else if (isTiles) {
				parseTileLine(line);
			}
		}
	}

	SingleLevel::~SingleLevel() {

	}

	void SingleLevel::update() {
		// is there anything to update?
	}

	void SingleLevel::render(SDL_Renderer* renderer) {
		for (int layerNum{ 0 }; layerNum < _tileRects.size(); layerNum++) {
			auto& tilesetTexture = _layerTilesetMap[layerNum];
			if (!tilesetTexture->getValue()) continue;
			
			auto& tileRectMap = _tileRects[layerNum];
			
			for (int levelHeight{ 0 }; levelHeight < _levelDimensions.second; levelHeight++) {
				if (!tileRectMap.count(levelHeight)) continue;
				
				auto& tileRectsVec = tileRectMap[levelHeight];
				for (int levelWidth{ 0 }; levelWidth < _levelDimensions.first; levelWidth++) {
					Client::Camera& camera{ Scenes::GameScene::instance->getCamera() };
					
					SDL_Rect& srcRect = tileRectsVec.at(levelWidth);
					
					if (srcRect.w == 0) continue; // blank
					SDL_Rect dstRect{ _tileSize * levelWidth - camera.getX() + camera.getOffsetX(), _tileSize * levelHeight - camera.getY() + camera.getOffsetY(), _tileSize, _tileSize};

					SDL_RenderCopy(renderer, tilesetTexture->getValue()->getUnderlyingTexture(), &srcRect, &dstRect);
				}
			}
		}
	}

	void SingleLevel::parseTileLine(std::string const& line) {
		int colonIndex{ static_cast<int>(line.find(':'))};
		int rowIndex{ std::stoi(line.substr(0, colonIndex)) };
		
		auto& map = _tileRects.back();
		map.insert(std::make_pair(rowIndex, std::vector<SDL_Rect>{}));
		auto& vec = map[rowIndex];
		
		for (int i{ 0 }; i < _levelDimensions.first*2; i += 2) {
			std::string b64Chars{ line.substr(colonIndex + 1 + i, colonIndex + 3 + i) };
			
			if (b64Chars == "--")
				vec.emplace_back();
			
			int tileIndex{ b64ToInt(b64Chars) };
			int tileX{ tileIndex % 128 * _tileSize };
			int tileY{ tileIndex / 128 * _tileSize };
			vec.emplace_back(tileX, tileY, _tileSize, _tileSize);
			//std::cout << "running?";
		}
	}
}