#include "MapLevel.hpp"
#include <string_view>
#include <SDL2/SDL.h>
#include <istream>
#include <boost/algorithm/string.hpp>
#include "Camera.hpp"

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
		line = line.substr(line.size() - 4); // Necessary if the file begins with BOM denoting utf-8
		_levelDimensions.first = b64ToInt(line.substr(0, 2));
		_levelDimensions.second = b64ToInt(line.substr(2));

		std::getline(iss, line); // second line (tile size)
		_tileSize = std::stoi(line);
		
		std::getline(iss, line);
		_renderDistance = std::stoi(line);
		
		_mapLevels.resize(_mapDimensions.second);
		for (int i{ 0 }; i < _mapDimensions.second; i++) {
			_mapLevels.emplace_back();
			_mapLevels.back().resize(_mapDimensions.first);
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
				_mapLevels[i][j] = line;
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
		
	}

	void MapLevel::render(SDL_Renderer* renderer) {}

	int MapLevel::getLevelHeight() {
		return _levelDimensions.second * _mapDimensions.second;
	}

	int MapLevel::getLevelWidth() {
		return _levelDimensions.first * _mapDimensions.first;
	}
}