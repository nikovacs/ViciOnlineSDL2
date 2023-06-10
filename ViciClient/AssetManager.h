#pragma once
#include "NetworkAsset.h"
#include <string>
#include <unordered_map>
#include <enet/enet.h>
#include <memory>

class AssetManager {
public:
	static void initializeIndex();
	template <unsigned int N>
	static void requestFileAsBytes(std::string_view);
	static void onBytesReceived(ENetEvent& event);
	static void writeFile(std::string_view filePath, std::string_view fileData);
	template <typename T>
	static void resolve(Networking::NetworkAsset<T>* asset);
private:
	static void createStructure();
	static std::unordered_map<std::string, std::string> _assetIndex; // <assetName, assetPath>
	static std::unordered_map<std::string, std::weak_ptr<void>> _assetCache; // <assetName, asset>
	//static std::unordered_map<std::string, std::weak_ptr<Networking::AbstractNetworkAsset>> _assetsInProgress;
};