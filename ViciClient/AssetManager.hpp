#pragma once
#include "UdpChannelMap.hpp"
#include <string>
#include <unordered_map>
#include <map>
#include <enet/enet.h>
#include <memory>
#include <iostream>
#include "../ViciEngine/AssetTransfer.hpp"
#include "../ViciEngine/AssetCache.hpp"
#include <mutex>

namespace Networking {
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
		static void onReceived(ENetEvent& event);

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
		static void retrieveAsset(std::string_view fileName) {
			if (_permanentAssets.contains(fileName.data())) return;
			{
				std::lock_guard<std::mutex> lock(_assetsInProgressMutex);
				if (_assetsInProgress.contains(fileName.data())) return;
			}
			if (_assetCache.contains<T>(fileName.data())) return;

			// add to _assetsInProgress in preparation for requesting from server
			{
				std::lock_guard<std::mutex> lock(_assetsInProgressMutex);
				_assetsInProgress.emplace(fileName.data(), nullptr);
			}
			int channelID{ Networking::UdpTypeChannelMap::template getChannel<T>() };
			requestFile(fileName.data(), channelID);
		}
	private:
		/**
		* Because onReceived can run on multiple different threads at the same time,
		* it is important for _assetsInProgress to be thread-safe and not invaidate any pointers.
		* An unordered_map can invalidate pointers if inserting requires re-hashing the elements,
		* which does not happen with a normal map.
		*/ 
		static std::map<std::string, std::shared_ptr<void>> _assetsInProgress; // <assetName, asset_ptr>
		static std::mutex _assetsInProgressMutex;

		//static TimedCache<std::string, void> _assetCache; // <assetName, asset>
		static AssetCache _assetCache;
		static std::unordered_map<std::string, std::shared_ptr<void>> _permanentAssets; // <assetName, asset_ptr>
	};
}