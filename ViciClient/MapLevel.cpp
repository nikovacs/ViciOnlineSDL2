#include "MapLevel.hpp"
#include <string_view>
#include <SDL2/SDL.h>
#include <istream>
#include <boost/algorithm/string.hpp>
#include "Camera.hpp"
#include "SingleLevel.hpp"
#include <memory>
#include "NetworkAsset.hpp"

namespace Levels {
	MapLevel::MapLevel(std::string_view name, std::string_view source) : Level{ name } {
		std::istringstream iss{ source.data() };
		std::string line;

		std::getline(iss, line); // first line (level dimensions)
		boost::trim(line);
		line = line.substr(line.size() - 4); // Necessary if the file begins with BOM denoting utf-8
		_mapDimensions.first = b64ToInt(line.substr(0, 2));
		_mapDimensions.second = b64ToInt(line.substr(2));

		std::getline(iss, line); // first line (level dimensions)
		boost::trim(line);
		//line = line.substr(line.size() - 4); // Necessary if the file begins with BOM denoting utf-8
		_levelDimensions.first = b64ToInt(line.substr(0, 2));
		_levelDimensions.second = b64ToInt(line.substr(2));

		std::getline(iss, line); // second line (tile size)
		_tileSize = std::stoi(line);
		
		std::getline(iss, line);
		_renderDistance = std::stoi(line);
		
		_mapLevels.reserve(_mapDimensions.second);
		for (int i{ 0 }; i < _mapDimensions.second; i++) {
			_mapLevels.emplace_back();
			_mapLevels.back().reserve(_mapDimensions.first);
		}
		bool recordLevels{ false };
		int i{ 0 }, j{ 0 };

		while (std::getline(iss, line)) {
			boost::trim(line);
			if (line.starts_with("sl")) {
				recordLevels = true;
			}
			else if (line.starts_with("el")) {
				recordLevels = false;
			}
			else if (recordLevels) {
				_mapLevels.at(i).emplace_back(line + ".vlvl");
				j++;
				if (j == _mapDimensions.first) {
					j = 0;
					i++;
				}
			}
		}
	}

	MapLevel::~MapLevel() {}
	
	void MapLevel::update() {
		updateFocusLevel();
		verifyAndUpdateLevelAssets();
		for (int i{ _focusLevel.first - _renderDistance }; i <= _focusLevel.first + _renderDistance; i++) {
			if (i < 0 || i >= _mapDimensions.first) continue;
			for (int j{ _focusLevel.second - _renderDistance }; j <= _focusLevel.second + _renderDistance; j++) {
				if (j < 0 || j >= _mapDimensions.second) continue;
				auto* lvl = _levelNameAssetMap.at(_mapLevels[j][i])->getValue();
				if (lvl) lvl->update();
			}
		}
	}

	void MapLevel::render(SDL_Renderer* renderer) {
		for (int i{ _focusLevel.first - _renderDistance }; i <= _focusLevel.first + _renderDistance; i++) {
			if (i < 0 || i >= _mapDimensions.first) continue;
			int xOffset{ _levelDimensions.second * _tileSize * i };
			
			for (int j{ _focusLevel.second - _renderDistance }; j <= _focusLevel.second + _renderDistance; j++) {
				if (j < 0 || j >= _mapDimensions.second) continue;
				if (!_levelNameAssetMap[_mapLevels[j][i]]->getValue()) continue;
				int yOffset{ _levelDimensions.first * _tileSize * j };

				reinterpret_cast<Levels::SingleLevel*>(_levelNameAssetMap[_mapLevels[j][i]]->getValue())->renderWithOffsets(renderer, xOffset, yOffset);
			}
		}
	}

	int MapLevel::getLevelHeight() {
		return _levelDimensions.second * _mapDimensions.second;
	}

	int MapLevel::getLevelWidth() {
		return _levelDimensions.first * _mapDimensions.first;
	}

	void MapLevel::getRenderedLevels(std::set<std::string>& lvlsOut) {
		for (int i{ _focusLevel.first - _renderDistance }; i <= _focusLevel.first + _renderDistance; i++) {
			if (i < 0 || i >= _mapDimensions.first) continue;
			for (int j{ _focusLevel.second - _renderDistance }; j <= _focusLevel.second + _renderDistance; j++) {
				if (j < 0 || j >= _mapDimensions.second) continue;
				lvlsOut.insert(_mapLevels[j][i]);
			}
		}
	}

	void MapLevel::updateFocusLevel() {
		if (!Client::Camera::instance) return;
		int focusX, focusY;
		std::tie(focusX, focusY) = Client::Camera::instance->getFocusPoint();
		int levelWidthPixels = _levelDimensions.first * _tileSize;
		int levelHeightPixels = _levelDimensions.second * _tileSize;
		_focusLevel.first = focusX / levelWidthPixels;
		_focusLevel.second = focusY / levelHeightPixels;
		setCurrentLevel(_mapLevels[_focusLevel.second][_focusLevel.first]);
	}

	void MapLevel::verifyAndUpdateLevelAssets() {
		for (int i{ _focusLevel.first - _renderDistance }; i <= _focusLevel.first + _renderDistance; i++) {
			if (i < 0 || i >= _mapDimensions.first) continue;
			
			for (int j{ _focusLevel.second - _renderDistance }; j <= _focusLevel.second + _renderDistance; j++) {
				if (j < 0 || j >= _mapDimensions.second) continue;
				
				std::string& levelName = _mapLevels.at(j).at(i);
				
				if (!_levelNameAssetMap.contains(levelName)) {
					_levelNameAssetMap.emplace(levelName, std::make_unique<Networking::NetworkAsset<Levels::Level>>(levelName));
				}
				else if (_levelNameAssetMap[levelName]->getValue()) {
					_levelNameAssetMap[levelName]->getValue()->update();
				}
			}
		}
	}
}