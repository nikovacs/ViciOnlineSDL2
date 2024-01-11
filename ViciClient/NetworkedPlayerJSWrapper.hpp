#pragma once
#include "NetworkedPlayer.hpp"
#include <v8.h>
#include <v8pp/context.hpp>
#include <unordered_map>

using namespace v8;

namespace JS {
	class NetworkedPlayerJSWrapper {
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
		v8::Local<v8::Proxy> getClientW();
	private:
		Local<Value> _proxyGet(Local<Name> property);
		Local<Value> _proxySet(Local<Name> property, Local<Value> value);
		Local<Value> _proxyDelete(Local<Name> property);
		v8::Global<v8::Object> _clientWTarget;
		v8::Global<v8::Object> _clientWHandler;
		v8::Global<v8::Proxy> _clientWProxy;
		v8pp::context* _ctx{ nullptr };
		Entities::NetworkedPlayer* _player{ nullptr };
	};
}