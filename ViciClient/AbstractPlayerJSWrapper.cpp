#include "AbstractPlayerJSWrapper.hpp"
#include <iostream>

namespace JS {
	AbstractPlayerJSWrapper::AbstractPlayerJSWrapper(v8pp::context* ctx, Entities::Entity* player) : _ctx{ ctx }, _player{ player } {
		v8::HandleScope scope{ ctx->isolate() };

		// create the proxy for clientW
		v8::Local<v8::Object> clientWTargetObj{ v8::Object::New(ctx->isolate()) };
		_clientWTarget.Reset(ctx->isolate(), clientWTargetObj);

		v8::Local<v8::Object> clientWHandlerObj{ v8::Object::New(ctx->isolate()) };
		_clientWHandler.Reset(ctx->isolate(), clientWHandlerObj);

		_proxyGetterHelper(clientWHandlerObj, &_player->getClientW());
		_proxySetterHelper(clientWHandlerObj, &_player->getClientW());
		_proxyDeleterHelper(clientWHandlerObj, &_player->getClientW());

		v8::Local<v8::Proxy> clientWAttrs{ v8::Proxy::New(ctx->impl(), clientWTargetObj, clientWHandlerObj).ToLocalChecked() };
		_clientWProxy.Reset(ctx->isolate(), clientWAttrs);

		// create the proxy for clientR
		v8::Local<v8::Object> clientRTargetObj{ v8::Object::New(ctx->isolate()) };
		_clientRTarget.Reset(ctx->isolate(), clientRTargetObj);

		v8::Local<v8::Object> clientRHandlerObj{ v8::Object::New(ctx->isolate()) };
		_clientRHandler.Reset(ctx->isolate(), clientRHandlerObj);

		_proxyGetterHelper(clientRHandlerObj, &_player->getClientR());
		_proxySetterHelper(clientRHandlerObj, &_player->getClientR());
		_proxyDeleterHelper(clientRHandlerObj, &_player->getClientR());

		v8::Local<v8::Proxy> clientRAttrs{ v8::Proxy::New(ctx->impl(), clientRTargetObj, clientRHandlerObj).ToLocalChecked() };
		_clientRProxy.Reset(ctx->isolate(), clientRAttrs);
	}

	AbstractPlayerJSWrapper::AbstractPlayerJSWrapper(const AbstractPlayerJSWrapper& other) {
		_clientWTarget.Reset(other._ctx->isolate(), other._clientWTarget.Get(other._ctx->isolate()));
		_clientWHandler.Reset(other._ctx->isolate(), other._clientWHandler.Get(other._ctx->isolate()));
		_clientWProxy.Reset(other._ctx->isolate(), other._clientWProxy.Get(other._ctx->isolate()));
		_clientRTarget.Reset(other._ctx->isolate(), other._clientRTarget.Get(other._ctx->isolate()));
		_clientRHandler.Reset(other._ctx->isolate(), other._clientRHandler.Get(other._ctx->isolate()));
		_clientRProxy.Reset(other._ctx->isolate(), other._clientRProxy.Get(other._ctx->isolate()));
		_player = other._player;
		_allowWrite = other._allowWrite;
		_ctx = other._ctx;
	}

	v8::Local<v8::Proxy> AbstractPlayerJSWrapper::getClientW() {
		return _clientRProxy.Get(_ctx->isolate());
	}

	v8::Local<v8::Proxy> AbstractPlayerJSWrapper::getClientR() {
		return _clientWProxy.Get(_ctx->isolate());
	}

	void AbstractPlayerJSWrapper::setAllowWrite(bool allowWrite) {
		_allowWrite = allowWrite;
	}

	v8::Local<v8::Value> AbstractPlayerJSWrapper::_proxyGet(v8::Local<v8::Name> property, Attributes* attrs) {
		std::string propertyName{ v8pp::from_v8<std::string>(_ctx->isolate(), property, "") };

		if (!attrs->has(propertyName)) {
			return v8::Undefined(_ctx->isolate());
		}

		auto& val{ attrs->get(propertyName) };

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

	v8::Local<v8::Value> AbstractPlayerJSWrapper::_proxySet(v8::Local<v8::Name> property, v8::Local<v8::Value> value, Attributes* attrs) {
		std::string propertyName{ v8pp::from_v8<std::string>(_ctx->isolate(), property, "") };
		if (propertyName.empty()) {
			return v8::Boolean::New(_ctx->isolate(), false);
		}

		// TODO: might want to also support arrays and maps (objects)
		if (value->IsBoolean()) {
			attrs->set(propertyName, v8pp::from_v8<bool>(_ctx->isolate(), value));
		}
		else if (value->IsInt32()) {
			attrs->set(propertyName, v8pp::from_v8<int32_t>(_ctx->isolate(), value));
		}
		else if (value->IsNumber()) {
			attrs->set(propertyName, v8pp::from_v8<double>(_ctx->isolate(), value));
		}
		else if (value->IsString()) {
			attrs->set<std::string>(propertyName, v8pp::from_v8<std::string>(_ctx->isolate(), value));
		}
		else if (value->IsNull()) {
			attrs->set(propertyName, nullptr);
		}
		else if (value->IsUndefined()) {
			_proxyDelete(property, attrs);
		}
		else {
			return v8::Boolean::New(_ctx->isolate(), false);
		}

		return v8::Boolean::New(_ctx->isolate(), true);
	}

	v8::Local<v8::Value> AbstractPlayerJSWrapper::_proxyDelete(v8::Local<v8::Name> property, Attributes* attrs) {
		std::string propertyName{ v8pp::from_v8<std::string>(_ctx->isolate(), property, "") };
		if (propertyName.empty()) {
			return v8::Boolean::New(_ctx->isolate(), false);
		}

		if (attrs->has(propertyName)) {
			attrs->remove(propertyName);
			return v8::Boolean::New(_ctx->isolate(), true);
		}
		return v8::Boolean::New(_ctx->isolate(), false);
	}

	void AbstractPlayerJSWrapper::_proxyGetterHelper(v8::Local<v8::Object> handler, Attributes* attrs) {
		handler->Set(_ctx->isolate()->GetCurrentContext(), v8pp::to_v8(_ctx->isolate(), "get"), v8pp::wrap_function(_ctx->isolate(), "",
			[this, attrs](const v8::FunctionCallbackInfo<v8::Value>& args) {
				v8::Local<v8::Name> property = args[1].As<v8::Name>();
				return _proxyGet(property, attrs);
			}
		));
	}

	void AbstractPlayerJSWrapper::_proxySetterHelper(v8::Local<v8::Object> handler, Attributes* attrs) {
		handler->Set(_ctx->isolate()->GetCurrentContext(), v8pp::to_v8(_ctx->isolate(), "set"), v8pp::wrap_function(_ctx->isolate(), "",
			[this, attrs](const v8::FunctionCallbackInfo<v8::Value>& args)->v8::Local<v8::Value> {
				if (_allowWrite) {
					v8::Local<v8::Name> property = args[1].As<v8::Name>();
					v8::Local<v8::Value> value = args[2];
					return _proxySet(property, value, attrs);
				}
				else {
					return v8::Boolean::New(args.GetIsolate(), false);
				}
			}
		));
	}

	void AbstractPlayerJSWrapper::_proxyDeleterHelper(v8::Local<v8::Object> handler, Attributes* attrs) {
		handler->Set(_ctx->isolate()->GetCurrentContext(), v8pp::to_v8(_ctx->isolate(), "deleteProperty"), v8pp::wrap_function(_ctx->isolate(), "",
			[this, attrs](const v8::FunctionCallbackInfo<v8::Value>& args)->v8::Local<v8::Value> {
				if (_allowWrite) {
					v8::Local<v8::Name> property = args[1].As<v8::Name>();
					return _proxyDelete(property, attrs);
				}
				else {
					return v8::Boolean::New(args.GetIsolate(), false);
				}
			}
		));
	}
}