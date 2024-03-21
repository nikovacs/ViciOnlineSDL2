#include "AssetManager.hpp"
#include "UdpClient.hpp"
#include "Animation.hpp"
#include "Gottimation.hpp"
#include "../ViciEngine/UdpChannels.hpp"
#include "../ViciEngine/base64.hpp"
#include "../ViciEngine/AssetTransfer.hpp"
#include "../ViciEngine/Script.hpp"
#include "ClientScriptLoader.hpp"
#include "UdpChannelMap.hpp"
#include <enet/enet.h>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <fstream>
#include <memory>
#include "Level.hpp"
#include "SingleLevel.hpp"
#include "MapLevel.hpp"
#include "TimeManager.hpp"

// Embedding assets
#include "vicigen_ApplestormChalkboard.h"
#include "vicigen_loginScene_css.h"
#include "vicigen_loginScene_html.h"
#include "vicigen_guiBackground_png.h"
#include "vicigen_vicionlinelogo_png.h"
#include "vicigen_registerScene_html.h"

namespace fs = std::filesystem;

TimedCache<std::string, void> Networking::AssetManager::_assetCache{};
std::unordered_map<std::string, std::shared_ptr<void>> Networking::AssetManager::_assetsInProgress{};
std::unordered_map<std::string, std::shared_ptr<void>> Networking::AssetManager::_permanentAssets{};

void Networking::AssetManager::generatePermanentAssets() {
	// if the following items are not retrieved exactly as written here (including case)
	// the client will attempt to retrieve them from the server
	// however, the connection to the server is not yet established at this point
	// so the client will not be able to retrieve them and will crash
	std::string content(reinterpret_cast<char const*>(ApplestormChalkboard_otf), ApplestormChalkboard_otf_len);
	_permanentAssets.emplace("ApplestormChalkboard.otf", std::make_shared<std::string>(content));

	content = std::string(reinterpret_cast<char const*>(loginScene_css), loginScene_css_len);
	_permanentAssets.emplace("__vici_internal__loginScene.css", std::make_shared<std::string>(content));
	
	content = std::string(reinterpret_cast<char const*>(loginScene_html), loginScene_html_len);
	_permanentAssets.emplace("__vici_internal__loginScene.html", std::make_shared<std::string>(content));

	content = std::string(reinterpret_cast<char const*>(guiBackground_png), guiBackground_png_len);
	_permanentAssets.emplace("__vici_internal__guiBackground.png", std::make_shared<AssetTypes::Texture>(content));

	content = std::string(reinterpret_cast<char const*>(vicionlinelogo_png), vicionlinelogo_png_len);
	_permanentAssets.emplace("__vici_internal__viciOnlineLogo.png", std::make_shared<AssetTypes::Texture>(content));

	content = std::string(reinterpret_cast<char const*>(registerScene_html), registerScene_html_len);
	_permanentAssets.emplace("__vici_internal__registerScene.html", std::make_shared<std::string>(content));
}

void Networking::AssetManager::requestFile(std::string_view filePath, int channelID) {
	std::cout << "Requesting " << filePath << std::endl;
	const uint8_t* buffer = reinterpret_cast<const uint8_t*>(filePath.data());
	size_t bufferSize = filePath.length();

	// Create an ENetPacket with the byte buffer
	ENetPacket* packet = enet_packet_create(buffer, bufferSize, ENET_PACKET_FLAG_RELIABLE);

	// Send the packet to the destination peer on the desired channel
	int ret; 
	// TODO: FIX BAD IMPLEMENTATION. AFTER SO MANY FAILED ATTEMPTS, THE CLIENT SHOULD TIME OUT AND DISCONNECT.
	// TODO: IT IS POSSIBLE THAT THIS IS DUE TO VISUAL STUDIO STARTING THE SERVER ONLY IMMEDIATELY BEFORE THE CLIENT AND THE SERVER DOES NOT HAVE ENOUGH TIME TO START
	do {
		ret = enet_peer_send(static_cast<Networking::UdpClient*>(Networking::UdpClient::instance)->getGameServer(), channelID, packet);
	} while (ret < 0);
}

void Networking::AssetManager::onReceived(ENetEvent& event) {
	auto json{ Networking::UdpClient::getJsonFromPacket(event.packet) };

	std::string path = json["path"];
	std::string fileData = json["data"];

	std::cout << "Received " << path << std::endl;
	addAssetToIndex(path);

	//load from path
	std::string_view typeName{ Networking::UdpTypeChannelMap::getTypeFromChannel(event.channelID) };
	std::shared_ptr<void>& assetInProgress = _assetsInProgress.at(path);

	size_t dotIndex{ path.find_last_of('.') };
	std::string extension{ path.substr(dotIndex + 1) };

	if (typeName == "Texture") {
		assetInProgress = std::make_shared<AssetTypes::Texture>(base64::from_base64(fileData));
	}
	else if (typeName == "Script") {
		assetInProgress = std::make_shared<JS::Script>(JS::ClientScriptLoader::instance->getIsolate(), base64::from_base64(fileData));
	}
	else if (typeName == "Animation") {
		if (extension == "vani") {
			assetInProgress = std::make_shared<Animations::Animation>(path, base64::from_base64(fileData));
		}
		else if (extension == "json") {
			assetInProgress = std::make_shared<Animations::Gottimation>(path, base64::from_base64(fileData));
		}
	}
	else if (typeName == "Level") {
		if (extension == "vlvl")
			assetInProgress = std::make_shared<Levels::SingleLevel>(path, base64::from_base64(fileData));
		else if (extension == "vmap") {
			assetInProgress = std::make_shared<Levels::MapLevel>(path, base64::from_base64(fileData));
		}
	}
	else if (typeName == "String") {
		assetInProgress = std::make_shared<std::string>(base64::from_base64(fileData));
	}
	else {
		throw std::runtime_error("Unknown type: " + std::string(typeName));
	}

	// put it into the cache
	_assetCache.emplace(path, _assetsInProgress.at(path));
	_assetCache.update();
}

void Networking::AssetManager::clearCache() {
	_assetCache.clear();
	_permanentAssets.clear();
	_assetsInProgress.clear();
}