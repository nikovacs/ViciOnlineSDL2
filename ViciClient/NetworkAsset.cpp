#include "NetworkAsset.h"
#include "AssetManager.h"

namespace Networking {
	template <typename T>
	NetworkAsset<T>::NetworkAsset(std::string_view assetName) : _fileName{ assetName }, _value{ nullptr }, _resolved { false } {
		//AssetManager::requestFileAsBytes<sizeof(T)>(assetName);
	}

	template <typename T>
	NetworkAsset<T>::NetworkAsset(std::string_view assetName, T val) : _fileName{ assetName }, _value{ val }, _resolved{ true } {
		//AssetManager::writeFile(assetName, val);
	}

	template <typename T>
	T* NetworkAsset<T>::getValue() {
		if (_resolved) {
			return _value.get();
		}
		AssetManager::resolve<T>(this);
		return _value.get();
	}

	template <typename T>
	std::string_view NetworkAsset<T>::getFileName() {
		return _fileName;
	}

	template <typename T>
	void NetworkAsset<T>::finish(std::shared_ptr<T> val) {
		_value = val;
		_resolved = true;
	}
}