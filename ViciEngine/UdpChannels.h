#pragma once
#include <map>
#include <typeindex>
#include "Texture.h"

namespace Networking {
	enum UdpChannels {
		Texture,
		Animation,
		Level,
		MAX_CHANNELS
	};

	class TypeChannelMap {
	private:
		static inline const std::map<std::type_index, int> _typeToNumberMap{
			{ std::type_index(typeid(AssetTypes::Texture)), Texture},
		};
	};
}