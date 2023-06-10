#pragma once

#include <string>
#include <memory>
#include <atomic>
#include "../ViciEngine/UdpChannels.h"

namespace Networking {
	class AbstractNetworkAsset {
	public:
	protected:
		std::string _fileName;
		std::atomic_bool _resolved;
	};
	
	template <typename T>
	class NetworkAsset : public AbstractNetworkAsset {
	public:
		NetworkAsset(std::string_view assetName);
		NetworkAsset(std::string_view assetName, T val);
		T* getValue();
		std::string_view getFileName();
		void finish(std::shared_ptr<T> val);
	private:
		std::shared_ptr<T> _value;
	};
}