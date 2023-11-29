#include "NetworkedPlayerJSWrapper.hpp"
#include <v8pp/context.hpp>
#include <v8pp/class.hpp>
#include <iostream>

using namespace v8;

namespace JS {
	NetworkedPlayerJSWrapper::NetworkedPlayerJSWrapper(Entities::NetworkedPlayer* pl, v8pp::context* ctx) : _player{ pl }, _ctx{ ctx } {
		// create a v8 proxy
		v8::HandleScope scope{ ctx->isolate() };

		v8::Local<v8::Object> targetObj = v8::Object::New(ctx->isolate());
		_clientWTarget.Reset(ctx->isolate(), targetObj);

		v8::Local<v8::Object> handlerObj = v8::Object::New(ctx->isolate());
		_clientWHandler.Reset(ctx->isolate(), handlerObj);
		
		handlerObj->Set(ctx->isolate()->GetCurrentContext(), v8pp::to_v8(ctx->isolate(), "get"), v8pp::wrap_function(ctx->isolate(), "",
			[this](Local<Object> target, Local<Name> property, Local<Object> receiver) {
				return _proxyGet(property);
			}
		));

		handlerObj->Set(ctx->isolate()->GetCurrentContext(), v8pp::to_v8(ctx->isolate(), "set"), v8pp::wrap_function(ctx->isolate(), "",
			[this](Local<Name> property, Local<Value> value) {
				return _proxySet(property, value);
			}
		));

		handlerObj->Set(ctx->isolate()->GetCurrentContext(), v8pp::to_v8(ctx->isolate(), "deleteProperty"), v8pp::wrap_function(ctx->isolate(), "",
			[this](Local<Name> property) {
				return _proxyDelete(property);
			}
		));
		
		v8::Local<v8::Proxy> clientWAttrs = v8::Proxy::New(ctx->impl(), targetObj, handlerObj).ToLocalChecked();
		_clientWProxy.Reset(ctx->isolate(), clientWAttrs);
	}

	std::string_view NetworkedPlayerJSWrapper::getUsername() {
		return _player->getUsername();
	}
	int NetworkedPlayerJSWrapper::getX() {
		return _player->getX();
	}
	int NetworkedPlayerJSWrapper::getY() {
		return _player->getY();
	}
	int NetworkedPlayerJSWrapper::getDir() {
		return _player->getDir();
	}
	std::string_view NetworkedPlayerJSWrapper::getAni() {
		return _player->getAni();
	}
	int NetworkedPlayerJSWrapper::getWidth() {
		return _player->getWidth();
	}
	int NetworkedPlayerJSWrapper::getHeight() {
		return _player->getHeight();
	}

	v8::Local<v8::Proxy> NetworkedPlayerJSWrapper::getClientW() {
		return _clientWProxy.Get(_ctx->isolate());
	}

	Local<Value> NetworkedPlayerJSWrapper::_proxyGet(Local<Name> property) {
		std::cout << "_proxyGet called" << std::endl;
		return v8::Boolean::New(_ctx->isolate(), true);
	}

	Local<Value> NetworkedPlayerJSWrapper::_proxySet(Local<Name> property, Local<Value> value) {
		std::cout << "_proxySet called" << std::endl;
		return v8::Boolean::New(_ctx->isolate(), true);
	}

	Local<Value> NetworkedPlayerJSWrapper::_proxyDelete(Local<Name> property) {
		std::cout << "_proxyDelete called" << std::endl;
		return v8::Boolean::New(_ctx->isolate(), true);
	}
}