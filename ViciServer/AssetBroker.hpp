#pragma once

#include <unordered_map>
#include <string>
#include <enet/enet.h>

class AssetBroker {
public:
	static void initializeIndex();
	static void sendFileAsBytes(ENetEvent& event);
private:
	static std::unordered_map<std::string, std::string> assetIndex; // <assetName, assetPath>
	static void createStructure(); // create file structure if it is not already there
};