#pragma once
#include "NetworkedPlayer.hpp"

namespace JS {
	class NetworkedPlayerJSWrapper {
	public:
		NetworkedPlayerJSWrapper();
		NetworkedPlayerJSWrapper(Entities::NetworkedPlayer*);
		virtual ~NetworkedPlayerJSWrapper() = default;
		NetworkedPlayerJSWrapper(const NetworkedPlayerJSWrapper&);
		std::string_view getUsername();
		int getX();
		int getY();
		int getDir();
		std::string_view getAni();
		int getWidth();
		int getHeight();
	private:
		Entities::NetworkedPlayer* _player;
	};
}