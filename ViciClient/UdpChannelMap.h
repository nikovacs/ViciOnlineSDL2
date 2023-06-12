#pragma once

#include <map>
#include <typeindex>
#include "Texture.h"
#include "../ViciEngine/UdpChannels.h"

namespace Networking {
	class TypeChannelMap {
	private:
		static inline const std::map<std::type_index, int> _typeToNumberMap{
			{ std::type_index(typeid(AssetTypes::Texture)), Texture},
		};
	public:
		template <typename T>
		static int getChannel() {
			std::type_index typeIndex(typeid(T));
			return _typeToNumberMap.at(typeIndex);
		}
	};
}