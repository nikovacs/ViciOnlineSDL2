#pragma once

#include <map>
#include <typeindex>
#include "Texture.h"
#include "../ViciEngine/UdpChannels.h"

namespace Networking {
	class UdpTypeChannelMap {
	private:
		static inline const std::map<std::type_index, int> _typeToNumberMap{
			{ std::type_index(typeid(AssetTypes::Texture)), Texture},
		};
		static inline const std::map<int, std::string> _numberToNameMap{
			{ Texture, "Texture" },
		};
	public:
		template <typename T>
		static int getChannel() {
			std::type_index typeIndex(typeid(T));
			return _typeToNumberMap.at(typeIndex);
		}
		 
		
		static std::string_view getTypeFromChannel(int channel) {
			return _numberToNameMap.at(channel);
		}
	};
}