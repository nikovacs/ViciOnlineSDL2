#pragma once
#include <memory>
#include <atomic>
#include <mutex>
#include "ViciCore/include/Utility/TryLockGuard.hpp"
#include "ViciClient/include/Networking/AssetManager.hpp"

namespace Networking {
	template <typename T>
	class NetworkAsset {
	public:
		NetworkAsset(std::string_view assetName) : _fileName{assetName}, _value { nullptr } {
			std::transform(_fileName.begin(), _fileName.end(), _fileName.begin(), ::tolower);

			AssetManager::registerNetworkAsset<T>(_fileName, this);
			std::shared_ptr<T> asset{ AssetManager::retrieveAsset<T>(_fileName) };
			if (asset) {
				resolve(asset);
			}
		}

		virtual ~NetworkAsset() {
			AssetManager::unregisterNetworkAsset<T>(_fileName, this);
		}

		NetworkAsset(const NetworkAsset&) = delete;
		NetworkAsset(NetworkAsset&&) = delete;

		T* getValue() {
			TryLockGuard lock{ _valueMutex };
			if (!lock) {
				return nullptr;
			}
			if (_newValue) {
				_value = _newValue;
				_newValue = nullptr;
			}
			return _value.get();
		}

		void resolve(std::shared_ptr<T> value) {
			std::lock_guard<std::mutex> lock(_valueMutex);
			_newValue = value;
		}

	private:
		std::shared_ptr<T> _value{ nullptr };
		std::shared_ptr<T> _newValue{ nullptr };
		std::mutex _valueMutex{};

		std::string _fileName{};
	};
}