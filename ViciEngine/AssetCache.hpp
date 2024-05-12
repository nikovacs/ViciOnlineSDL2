#pragma once

#include <string>
#include <memory>
#include <unordered_map>

class AssetCache {
public:
	AssetCache() = default;
	virtual ~AssetCache() = default;

	/**
	* Returns true or false whether the cache contains the asset with the given key.
	* 
	* @tparam T The type of asset to check for.
	* @param key The key of the asset to check for.
	* @return True if the asset was found, false otherwise.
	*/
	template <typename T>
	bool contains(const std::string& key) const {
		std::string type = typeid(T).name();
		if (_assets.contains(type)) {
			return _assets.at(type).contains(key);
		}
		return false;
	}

	/**
	* Adds an asset to the cache with the given key and asset type
	* 
	* @tparam T The type of asset to add.
	* @param key The key of the asset to add.
	* @param asset The asset to add.
	*/
	template <typename T>
	void add(const std::string& fileName, std::shared_ptr<T> asset) {
		std::string type = typeid(T).name();
		// the inner map will be created if it doesn't exist
		_assets[type][fileName] = asset;
	}

	/**
	* Overload allowing for assetType to be provided manually as a string
	* 
	* @param assetType The type of asset to add.
	* @param fileName The key of the asset to add.
	* @param asset The asset to add.
	*/
	void add(const std::string& assetType, const std::string& fileName, std::shared_ptr<void> asset);

	/**
	* Returns the asset with the given key.
	* 
	* @tparam T The type of asset to get.
	* @param key The key of the asset to get.
	* 
	* @return True if the asset was found, false otherwise.
	*/
	template <typename T>
	bool remove(const std::string& key) {
		std::string type = typeid(T).name();
		if (_assets.contains(type)) {
			if (_assets.at(type).contains(key)) {
				_assets.at(type).erase(key);

				// it is not important to remove the inner map, even if it is empty
				// there are only so many asset types, odds are it will be used again
				// if it was already used once
				return true;
			}
		}
		return false;
	}

	/**
	* Removes all assets with the key from the cache, regardless of asset type
	*/
	bool removeAll(const std::string& key);

	/**
	* Returns the asset with the given key.
	* 
	* @tparam T The type of asset to get.
	* @param key The key of the asset to get.
	* @return The asset with the given key.
	*/
	template <typename T>
	std::shared_ptr<T> get(const std::string& key) const {
		std::string type = typeid(T).name();
		if (_assets.contains(type)) {
			if (_assets.at(type).contains(key)) {
				return std::static_pointer_cast<T>(_assets.at(type).at(key));
			}
		}
		return nullptr;
	}

	/**
	* Remove all assets from the cache
	*/
	void clear();
private:
	// outter map: key = asset type, inner map: key = asset key, value = asset
	std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<void>>> _assets{};
};