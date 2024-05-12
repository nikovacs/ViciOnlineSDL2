#include "AssetCache.hpp"

void AssetCache::add(const std::string& assetType, const std::string& fileName, std::shared_ptr<void> asset) {
	_assets[assetType][fileName] = asset;
}

bool AssetCache::removeAll(const std::string& key) {
	bool removed = false;
	for (auto& [type, assets] : _assets) {
		if (assets.contains(key)) {
			assets.erase(key);
			removed = true;
		}
	}
	return removed;
}

void AssetCache::clear() {
	_assets.clear();
}