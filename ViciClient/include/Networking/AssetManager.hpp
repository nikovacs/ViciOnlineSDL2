#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "header_wrappers/enet_wrapper.h"
#include <memory>
#include <iostream>
#include <mutex>
#include "ViciCore/include/Networking/AssetTransfer.hpp"
#include "ViciCore/include/Utility/AssetCache.hpp"
#include "ViciClient/include/Networking/UdpChannelMap.hpp"

namespace Networking {
	// forward declaration to avoid circular dependency
	// template <typename T>
	// class NetworkAsset {
	// public:
	// 	void resolve(std::shared_ptr<T> value);
	// };


	class AssetManager : public AssetTransfer {
	public:
		/**
		* This method is used to generate permanent assets that are added at compile-time
		*/
		static void generatePermanentAssets();

		/**
		* This method is called to request a file from the server
		* @param fileName the name of the file to request
		* @param channelID the channel to send the request on
		*/
		static void requestFile(std::string_view, int channelID);

		/**
		* This method is called from the networking thread
		* upon receiving a file response from the server
		* 
		* @param event the event containing the file response
		*/
		static void onReceived(SimplePacket& packet, UdpChannels channel);

		/**
		* Clears the cache of all assets
		* Includes the _permanentAssets, _assetCache, and _assetsInProgress
		*/
		static void clearCache();

		/**
		* Called from within NetworkAsset when the NetworkAsset is attempted
		* to be used, but is not yet available. Therefore checks whether it has
		* become available for use.
		* 
		* @tparam T the type of asset to resolve
		* @param fileName the name of the file to resolve
		* @return the asset if it is available, otherwise nullptr
		*/
		template <typename T>
		static std::shared_ptr<T> resolve(std::string_view fileName) {
			if (_permanentAssets.contains(fileName.data()))
				return static_pointer_cast<T>(_permanentAssets.at(fileName.data()));
			if (_assetCache.contains<T>(fileName.data())) {
				return _assetCache.get<T>(fileName.data());
			}
			return nullptr;
		}

		/**
		* This method is called on construction of a NetworkAsset
		* If the method has not yet been requested, it will be requested
		* 
		* @tparam T the type of asset to request
		* @param fileName the name of the file to request
		*/
		template <typename T>
		static std::shared_ptr<T> retrieveAsset(std::string_view fileName) {
			std::string assetType{ typeid(T).name() };
			if (_permanentAssets.contains(fileName.data()))
				return static_pointer_cast<T>(_permanentAssets.at(fileName.data()));;

			{
				// locking before querying the assetCache prevents from requesting the same asset multiple times
				std::lock_guard<std::mutex> lock(_assetsInProgressMutex);

				if (_assetCache.contains<T>(fileName.data()))
					return _assetCache.get<T>(fileName.data());

				if (_assetsInProgress[assetType].contains(fileName.data()))
					return nullptr;
			}

			// add to _assetsInProgress in preparation for requesting from server
			{
				std::lock_guard<std::mutex> lock(_assetsInProgressMutex);
				_assetsInProgress[assetType].insert(fileName.data());
			}
			int channelID{ Networking::UdpTypeChannelMap::template getChannel<T>() };
			requestFile(fileName.data(), channelID);
			return nullptr;
		}

		/**
		* Remove all assets from the cache with the key fileName
		* @param fileName the name of the file to remove from the cache
		* @return true if the asset was removed, false otherwise
		*/
		static bool removeFromCache(std::string_view fileName);

		/**
		* Register a NetworkAsset with the AssetManager
		* This is required for the AssetManager to be able to invalidate resources at runtime
		* This is necessary if a resource is updated at runtime.
		* 
		* @tparam T the assetType of the NetworkAsset
		* @param fileName the name of the file to register
		* @param networkAsset the NetworkAsset to register
		*/
		template <typename T>
		static void registerNetworkAsset(std::string_view fileName, NetworkAsset<T>* networkAsset) {
			std::string assetType{ typeid(T).name() };
			std::lock_guard<std::mutex> lock(_networkAssetMutex);
			_networkAssets[assetType][fileName.data()].push_back(reinterpret_cast<NetworkAsset<void>*>(networkAsset));
		}

		/**
		* Unregister a NetworkAsset with the AssetManager
		* Typically called from the destructor of the NetworkAsset
		* 
		* @tparam T the assetType of the NetworkAsset
		* @param fileName the name of the file to unregister
		* @param networkAsset the NetworkAsset to unregister
		*/
		template <typename T>
		static void unregisterNetworkAsset(std::string_view fileName, NetworkAsset<T>* networkAsset) {
			std::string assetType{ typeid(T).name() };
			std::lock_guard<std::mutex> lock(_networkAssetMutex);
			auto& assets = _networkAssets[assetType][fileName.data()];
			assets.erase(std::remove(assets.begin(), assets.end(), reinterpret_cast<NetworkAsset<void>*>(networkAsset)), assets.end());

			if (assets.empty()) {
				_networkAssets[assetType].erase(fileName.data());
			}
		}

		/**
		* Called to update the NetworkAsset with the new asset
		* 
		* @tparam T the assetType of the NetworkAsset
		* @param fileName the name of the file to update
		* @param asset the new asset to update with
		*/
		template <typename T>
		static void resolveNetworkAssets(std::string_view fileName, std::shared_ptr<T> asset) {
			std::string assetType{ typeid(T).name() };
			std::lock_guard<std::mutex> lock(_networkAssetMutex);
			auto& assets = _networkAssets[assetType][fileName.data()];
			for (NetworkAsset<T>* networkAsset : assets) {
				networkAsset->resolve(asset);
			}
		}

	private:
		static std::unordered_map<std::string, std::unordered_set<std::string>> _assetsInProgress;
		static std::mutex _assetsInProgressMutex;

		static AssetCache _assetCache;
		static std::unordered_map<std::string, std::shared_ptr<void>> _permanentAssets; // <assetName, asset_ptr>

		static std::mutex _networkAssetMutex;
		static std::unordered_map<std::string, std::unordered_map<std::string, std::vector<NetworkAsset<void>*>>> _networkAssets; // <assetType, <assetName, NetworkAsset&>>
	};
}