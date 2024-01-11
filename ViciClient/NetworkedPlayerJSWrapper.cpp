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
			[this](const v8::FunctionCallbackInfo<v8::Value>& args) {
				Local<Name> property = args[1].As<Name>();
				return _proxyGet(property);
			}
		));

		handlerObj->Set(ctx->isolate()->GetCurrentContext(), v8pp::to_v8(ctx->isolate(), "set"), v8pp::wrap_function(ctx->isolate(), "",
			[this](const v8::FunctionCallbackInfo<v8::Value>& args) {
				Local<Name> property = args[1].As<Name>();
				Local<Value> value = args[2];
				return _proxySet(property, value);
			}
		));

		handlerObj->Set(ctx->isolate()->GetCurrentContext(), v8pp::to_v8(ctx->isolate(), "deleteProperty"), v8pp::wrap_function(ctx->isolate(), "",
			[this](const v8::FunctionCallbackInfo<v8::Value>& args) {
				Local<Name> property = args[0].As<Name>();
				return _proxyDelete(property);
			}
		));
		
		v8::Local<v8::Proxy> clientWAttrs = v8::Proxy::New(ctx->impl(), targetObj, handlerObj).ToLocalChecked();
		_clientWProxy.Reset(ctx->isolate(), clientWAttrs);
	}

	std::string NetworkedPlayerJSWrapper::getUsername() {
		return std::string(_player->getUsername());
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
		std::string propertyName{ v8pp::from_v8<std::string>(_ctx->isolate(), property, "") };

		nlohmann::json& clientW{ *_player->getClientW().getUnderlyingJson()};

		if (!clientW.contains(propertyName)) {
			return v8::Undefined(_ctx->isolate());
		}

		auto& val { clientW[propertyName] };
		if (val.is_number_integer()) {
			return v8pp::to_v8(_ctx->isolate(), val.get<int32_t>());
		}
		if (val.is_number_float()) {
			return v8pp::to_v8(_ctx->isolate(), val.get<double>());
		}
		if (val.is_boolean()) {
			return v8pp::to_v8(_ctx->isolate(), val.get<bool>());
		}
		if (val.is_string()) {
			return v8pp::to_v8(_ctx->isolate(), val.get<std::string>());
		}
		if (val.is_null()) {
			return v8::Null(_ctx->isolate());
		}

		return v8::Undefined(_ctx->isolate());
	}

	Local<Value> NetworkedPlayerJSWrapper::_proxySet(Local<Name> property, Local<Value> value) {
		std::string propertyName{ v8pp::from_v8<std::string>(_ctx->isolate(), property, "") };
		if (propertyName.empty()) {
			return v8::Boolean::New(_ctx->isolate(), false);
		}

		nlohmann::json& clientW{ *_player->getClientW().getUnderlyingJson()};

		// TODO: might want to also support arrays and maps (objects)
		if (value->IsBoolean()) {
			clientW[propertyName] = v8pp::from_v8<bool>(_ctx->isolate(), value);
		}
		else if (value->IsInt32()) {
			clientW[propertyName] = v8pp::from_v8<int32_t>(_ctx->isolate(), value);
		}
		else if (value->IsNumber()) {
			clientW[propertyName] = v8pp::from_v8<double>(_ctx->isolate(), value);
		}
		else if (value->IsString()) {
			clientW[propertyName] = v8pp::from_v8<std::string>(_ctx->isolate(), value);
		}
		else if (value->IsNull()) {
			clientW[propertyName] = nullptr;
		}
		else if (value->IsUndefined()) {
			_proxyDelete(property);
		}
		else {
			return v8::Boolean::New(_ctx->isolate(), false);
		}

		return v8::Boolean::New(_ctx->isolate(), true);
	}

	Local<Value> NetworkedPlayerJSWrapper::_proxyDelete(Local<Name> property) {
		std::string propertyName{ v8pp::from_v8<std::string>(_ctx->isolate(), property, "") };
		if (propertyName.empty()) {
			return v8::Boolean::New(_ctx->isolate(), false);
		}

		nlohmann::json& clientW{ *_player->getClientW().getUnderlyingJson() };

		if (clientW.contains(propertyName)) {
			clientW.erase(propertyName);
			return v8::Boolean::New(_ctx->isolate(), true);
		}
		return v8::Boolean::New(_ctx->isolate(), false);
	}
}