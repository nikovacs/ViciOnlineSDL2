#pragma once
#include <string_view>
#include <string>
#include "header_wrappers/v8_wrapper.h"
#include <memory>
#include <map>
#include <v8-context.h>
#include <vector>
#include <functional>
#include "header_wrappers/v8pp_wrapper.h"

namespace JS {
	class Script {
	public:
		Script(v8::Isolate* isolate, std::string_view source);
		virtual ~Script();
		void initialize(std::function<void(v8pp::context* ctx)> apiSetupFunc);
		void run();

		template <typename... Args>
		void trigger(std::string_view functionName, Args&&... args) {
			v8::Isolate::Scope isolateScope(_isolate);
			v8::HandleScope handleScope(_isolate);

			v8::Local<v8::Value> func_value;
			_context->global();
			if (_context->global()->Get(_context->impl(), v8pp::to_v8(_context->isolate(), functionName)).ToLocal(&func_value) && func_value->IsFunction()) {
				v8::Local<v8::Function> function = v8::Local<v8::Function>::Cast(func_value);
				v8pp::call_v8(_isolate, function, _context->impl()->Global(), std::forward<Args>(args)...);
			}
		}
	protected:
		v8::Isolate* _isolate;
		std::string _fileName;
		std::string _source;
		v8::UniquePersistent<v8::Script> _script;
		std::unique_ptr<v8pp::context> _context{ nullptr };
	};
}