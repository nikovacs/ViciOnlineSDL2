#include "header_wrappers/enet_wrapper.h"
#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include "ViciCore/include/Networking/UdpChannels.hpp"
#include "ViciCore/include/Networking/AssetTransfer.hpp"
#include "ViciCore/include/JSRuntime/Script.hpp"
#include "ViciClient/include/TimeManager.hpp"
#include "ViciClient/include/Levels/Level.hpp"
#include "ViciClient/include/Levels/SingleLevel.hpp"
#include "ViciClient/include/Levels/MapLevel.hpp"
#include "ViciClient/include/Networking/AssetManager.hpp"
#include "ViciClient/include/Networking/UdpClient.hpp"
#include "ViciClient/include/Networking/UdpChannelMap.hpp"
#include "ViciClient/include/JSRuntime/ClientScriptLoader.hpp"

// Embedding assets
#include "ViciClient/vicigen/ApplestormChalkboard.otf.h"
#include "ViciClient/vicigen/loginScene.css.h"
#include "ViciClient/vicigen/loginScene.html.h"
#include "ViciClient/vicigen/guiBackground.png.h"
#include "ViciClient/vicigen/vicionlinelogo.png.h"
#include "ViciClient/vicigen/registerScene.html.h"

namespace fs = std::filesystem;

AssetCache Networking::AssetManager::_assetCache{};

std::unordered_map<std::string, std::unordered_set<std::string>> Networking::AssetManager::_assetsInProgress{};
std::mutex Networking::AssetManager::_assetsInProgressMutex{};

std::unordered_map<std::string, std::shared_ptr<void>> Networking::AssetManager::_permanentAssets{};

std::mutex Networking::AssetManager::_networkAssetMutex{};
std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Networking::NetworkAsset<void>*>>> Networking::AssetManager::_networkAssets{};

void Networking::AssetManager::generatePermanentAssets() {
	// ensure the file names are lowercase
	std::string content(reinterpret_cast<char const*>(ApplestormChalkboard_otf), ApplestormChalkboard_otf_len);
	_permanentAssets.emplace("applestormchalkboard.otf", std::make_shared<std::string>(content));

	content = std::string(reinterpret_cast<char const*>(loginScene_css), loginScene_css_len);
	_permanentAssets.emplace("__vici_internal__loginscene.css", std::make_shared<std::string>(content));
	
	content = std::string(reinterpret_cast<char const*>(loginScene_html), loginScene_html_len);
	_permanentAssets.emplace("__vici_internal__loginscene.html", std::make_shared<std::string>(content));

	content = std::string(reinterpret_cast<char const*>(guiBackground_png), guiBackground_png_len);
	_permanentAssets.emplace("__vici_internal__guibackground.png", std::make_shared<AssetTypes::Texture>(content));

	content = std::string(reinterpret_cast<char const*>(vicionlinelogo_png), vicionlinelogo_png_len);
	_permanentAssets.emplace("__vici_internal__vicionlinelogo.png", std::make_shared<AssetTypes::Texture>(content));

	content = std::string(reinterpret_cast<char const*>(registerScene_html), registerScene_html_len);
	_permanentAssets.emplace("__vici_internal__registerscene.html", std::make_shared<std::string>(content));
}

void Networking::AssetManager::requestFile(std::string_view fileName, int channelID) {
	SimplePacket packet{};
	packet.add<std::string>(fileName.data());

	Networking::UdpClient::sendSimplePacket(packet, static_cast<UdpChannels>(channelID), ENET_PACKET_FLAG_RELIABLE);
}

void Networking::AssetManager::onReceived(SimplePacket& packet, UdpChannels channel) {
	std::string assetType{};

	std::string fileName = packet.get<std::string>();
	std::string path = packet.get<std::string>();
	std::string fileData = packet.get<std::string>();

	std::cout << "Received " << fileName << std::endl;

	writeFile(path, fileData);
	_assetIndex.emplace(fileName, path);

	std::string_view typeName{ Networking::UdpTypeChannelMap::getTypeFromChannel(channel) };

	std::shared_ptr<void> assetInProgress{ nullptr };

	size_t dotIndex{ fileName.find_last_of('.') };
	std::string extension{ fileName.substr(dotIndex + 1) };

	if (typeName == "Texture") {
		assetInProgress = std::make_shared<AssetTypes::Texture>(fileData);
		assetType = typeid(AssetTypes::Texture).name();
	}
	else if (typeName == "Script") {
		assetInProgress = std::make_shared<JS::Script>(JS::ClientScriptLoader::instance->getIsolate(), fileData);
		assetType = typeid(JS::Script).name();
	}
	else if (typeName == "Json") {
		assetInProgress = std::make_shared<nlohmann::json>(nlohmann::json::parse(fileData));
		assetType = typeid(nlohmann::json).name();
	}
	else if (typeName == "Level") {
		if (extension == "vlvl")
			assetInProgress = std::make_shared<Levels::SingleLevel>(fileName, fileData);
		else if (extension == "vmap") {
			assetInProgress = std::make_shared<Levels::MapLevel>(fileName, fileData);
		}
		assetType = typeid(Levels::Level).name();
	}
	else if (typeName == "String") {
		assetInProgress = std::make_shared<std::string>(fileData);
		assetType = typeid(std::string).name();
	}
	else {
		throw std::runtime_error("Unknown type: " + std::string(typeName));
	}

	_assetCache.add(assetType, fileName, assetInProgress);
	//_assetCache.update(); // can be re-added if timing is re-added

	{
		std::lock_guard<std::mutex> lock(_networkAssetMutex);
		auto& assets = _networkAssets[assetType][fileName];
		for (auto* networkAsset : assets) {
			networkAsset->resolve(assetInProgress);
		}
	}

	{
		std::lock_guard<std::mutex> lock(_assetsInProgressMutex);
		_assetsInProgress[assetType].erase(fileName);
	}
}

void Networking::AssetManager::clearCache() {
	_assetCache.clear();
	_permanentAssets.clear();
	_assetsInProgress.clear();
}

bool Networking::AssetManager::removeFromCache(std::string_view fileName) {
	return _assetCache.removeAll(fileName.data());
	// at this point, only the shared_ptrs owned by the NetworkAssets still contain the asset
}
