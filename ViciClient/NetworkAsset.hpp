#pragma once
#include "AssetManager.hpp"
#include <memory>
#include <atomic>
#include <iostream>
namespace Networking {
	template <typename T>
	class NetworkAsset {
	public:
		NetworkAsset(std::string_view assetName) : _fileName{ assetName }, _value{ nullptr }, _resolved{ false } {
			AssetManager::retrieveAsset<T>(assetName);
		}

		T* getValue() {
			if (_resolved) {
				return _value.get();
			}
			_value = AssetManager::resolve<T>(_fileName);
			if (_value) _resolved = true;
			return _value.get();
		}

		void finish(std::shared_ptr<T> val) {
			_value = val;
			_resolved = true;
		}

	private:
		std::shared_ptr<T> _value;
		std::string _fileName;
		std::atomic_bool _resolved;
	};
}