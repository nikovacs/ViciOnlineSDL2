#include "Level.hpp"
#include <string_view>

namespace Levels {
	Level::Level(std::string_view name) : _name{ name } {}
	
	int Level::getRenderDistance() {
		return _renderDistance;
	}
	
	int Level::getTileSize() {
		return _tileSize;
	}
	
	int Level::getLevelHeight() {
		return _levelDimensions.second;
	}

	int Level::getLevelWidth() {
		return _levelDimensions.first;
	}
}