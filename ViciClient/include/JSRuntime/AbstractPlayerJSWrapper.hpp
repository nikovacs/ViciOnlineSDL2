#pragma once

#include <v8.h>
#include <v8pp/context.hpp>
#include "ViciCore/include/Attributes/Attributes.hpp"
#include "ViciCore/include/Entities/Entity.hpp"

namespace JS {
	class AbstractPlayerJSWrapper {
	public:
		AbstractPlayerJSWrapper(v8pp::context* ctx, Entities::Entity* player);
		AbstractPlayerJSWrapper(const AbstractPlayerJSWrapper& other);
		virtual ~AbstractPlayerJSWrapper() = default;
		v8::Local<v8::Proxy> getClientW();
		v8::Local<v8::Proxy> getClientR();
	protected:
		v8pp::context* _ctx = nullptr;
		void setAllowWriteClientW(bool allow);
	private:
		v8::Local<v8::Value> _proxyGet(v8::Local<v8::Name> property, Attributes* attrs);
		v8::Local<v8::Value> _proxySet(v8::Local<v8::Name> property, v8::Local<v8::Value> value, Attributes* attrs);
		v8::Local<v8::Value> _proxyDelete(v8::Local<v8::Name> property, Attributes* attrs);
		void _proxyGetterHelper(v8::Local<v8::Object> handler, Attributes* attrs);
		void _proxySetterHelper(v8::Local<v8::Object> handler, Attributes* attrs, bool isClientW);
		void _proxyDeleterHelper(v8::Local<v8::Object> handler, Attributes* attrs, bool isClientW);
		v8::Global<v8::Object> _clientWTarget;
		v8::Global<v8::Object> _clientWHandler;
		v8::Global<v8::Proxy> _clientWProxy;
		v8::Global<v8::Object> _clientRTarget;
		v8::Global<v8::Object> _clientRHandler;
		v8::Global<v8::Proxy> _clientRProxy;
		Entities::Entity* _player = nullptr;
		bool _allowWriteClientW = false;
	};
}