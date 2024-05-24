#pragma once

#include "ServerPlayer.hpp"

namespace JS {
	class ServerPlayerJSWrapper {
	public:
		ServerPlayerJSWrapper(Entities::ServerPlayer* pl);
		virtual ~ServerPlayerJSWrapper() = default;
		void addScript(std::string script);
		void removeScript(std::string script);
	private:
		Entities::ServerPlayer* _player{ nullptr };
	};
}