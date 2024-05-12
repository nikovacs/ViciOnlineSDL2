#pragma once
#include "UdpChannelMap.hpp"
#include <string>
#include <unordered_map>
#include <enet/enet.h>
#include <memory>
#include <iostream>
#include "../ViciEngine/AssetTransfer.hpp"
#include "../ViciEngine/AssetCache.hpp"

namespace Networking {
	class AssetManager : public AssetTransfer {
	public:
		static void generatePermanentAssets();
		static void requestFile(std::string_view, int channelID);
		static void onReceived(ENetEvent& event);
		static void clearCache();

		template <typename T>
		static std::shared_ptr<T> resolve(std::string_view fileName) {
			if (_assetCache.contains<T>(fileName.data()))
				return _assetCache.get<T>(fileName.data());
			else if (_permanentAssets.contains(fileName.data()))
				return static_pointer_cast<T>(_permanentAssets.at(fileName.data()));
			return nullptr;
		}

		template <typename T>
		static void retrieveAsset(std::string_view fileName) {
			if (_assetsInProgress.contains(fileName.data())) return;
			if (_assetCache.contains<T>(fileName.data())) return;
			if (_permanentAssets.contains(fileName.data())) return;

			// add to _assetsInProgress in preparation for requesting from server
			_assetsInProgress.emplace(fileName.data(), nullptr);
			int channelID{ Networking::UdpTypeChannelMap::template getChannel<T>() };
			requestFile(fileName.data(), channelID);
		}
	private:
		static std::unordered_map<std::string, std::shared_ptr<void>> _assetsInProgress; // <assetName, asset_ptr>
		//static TimedCache<std::string, void> _assetCache; // <assetName, asset>
		static AssetCache _assetCache;
		static std::unordered_map<std::string, std::shared_ptr<void>> _permanentAssets; // <assetName, asset_ptr>
	};
}