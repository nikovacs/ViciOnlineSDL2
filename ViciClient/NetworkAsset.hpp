#pragma once
#include "AssetManager.hpp"
#include <memory>
#include <atomic>
#include <iostream>
namespace Networking {
	template <typename T>
	class NetworkAsset {
	public:
		NetworkAsset(std::string_view assetName) : _fileName{assetName}, _value { nullptr } {
			std::transform(_fileName.begin(), _fileName.end(), _fileName.begin(), ::tolower);
			AssetManager::retrieveAsset<T>(_fileName);
		}

		T* getValue() {
			if (_value) {
				return _value.get();
			}
			_value = AssetManager::resolve<T>(_fileName);
			return _value.get();
		}

	private:
		std::shared_ptr<T> _value{ nullptr };
		std::string _fileName{};
	};
}