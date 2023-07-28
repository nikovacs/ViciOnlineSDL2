#pragma once
#include "UdpChannelMap.hpp"
#include <string>
#include <unordered_map>
#include <enet/enet.h>
#include <memory>
#include <iostream>
#include "../ViciEngine/AssetTransfer.hpp"

namespace Networking {
	class AssetManager : public AssetTransfer {
	public:
		static void requestFile(std::string_view, int channelID);
		static void onReceived(ENetEvent& event);

		template <typename T>
		static std::shared_ptr<T> resolve(std::string_view fileName) {
			if (_assetCache.contains(fileName.data())) {
				if (!_assetCache[fileName.data()].expired()) {
					std::shared_ptr<T> return_ptr = static_pointer_cast<T>(_assetCache[fileName.data()].lock());
					_assetsInProgress.erase(fileName.data());
					return return_ptr;
				}
				_assetCache.erase(fileName.data()); // Right now, just remove from cache if no references are left
			}
			return nullptr;
		}

		template <typename T>
		static void retrieveAsset(std::string_view fileName) {
			if (_assetsInProgress.contains(fileName.data()) || _assetCache.contains(fileName.data())) {
				// asset is already in progress, just wait for it to finish.
				return;
			}
			if (_assetCache.count(fileName.data())) {
				if (_assetCache[fileName.data()].expired()) {
					_assetCache.erase(fileName.data());
				}
			}

			// add to _assetsInProgress in preparation for requesting from server
			_assetsInProgress.emplace(fileName.data(), nullptr);
			int channelID{ Networking::UdpTypeChannelMap::template getChannel<T>() };
			requestFile(fileName.data(), channelID);
		}
	private:
		static std::unordered_map<std::string, std::shared_ptr<void>> _assetsInProgress; // <assetName, asset_ptr>
		static std::unordered_map<std::string, std::weak_ptr<void>> _assetCache; // <assetName, asset>
	};
}