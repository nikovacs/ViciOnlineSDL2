#include "Level.hpp"

namespace Levels {
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