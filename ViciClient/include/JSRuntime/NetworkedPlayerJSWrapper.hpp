#pragma once

#include <v8.h>
#include <v8pp/context.hpp>
#include <unordered_map>
#include "ViciCore/include/Attributes/Attributes.hpp"
#include "ViciClient/include/Entities/NetworkedPlayer.hpp"
#include "ViciClient/include/JSRuntime/AbstractPlayerJSWrapper.hpp"

namespace JS {
	class NetworkedPlayerJSWrapper : public AbstractPlayerJSWrapper {
	public:
		NetworkedPlayerJSWrapper(Entities::NetworkedPlayer*, v8pp::context* ctx);
		virtual ~NetworkedPlayerJSWrapper() = default;
		std::string getUsername();
		int getX();
		int getY();
		int getDir();
		std::string_view getAni();
		int getWidth();
		int getHeight();
	private:
		Entities::NetworkedPlayer* _player{ nullptr };
	};
}