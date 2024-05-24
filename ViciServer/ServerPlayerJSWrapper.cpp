#include "ServerPlayerJSWrapper.hpp"

namespace JS {
	ServerPlayerJSWrapper::ServerPlayerJSWrapper(Entities::ServerPlayer* pl) : _player(pl) {}

	void ServerPlayerJSWrapper::addScript(std::string script) {
		_player->addScript(script);
	}

	void ServerPlayerJSWrapper::removeScript(std::string script) {
		_player->removeScript(script);
	}
}