#pragma once
#include <string_view>
#include <string>
#include <v8.h>
#include <memory>
#include <map>
#include <vector>
#include <functional>
#include <v8pp/context.hpp>

namespace JS {
	class Script {
	public:
		Script(v8::Isolate* isolate, std::string_view source);
		virtual ~Script();
		void initialize(std::function<void(v8pp::context* ctx)> apiSetupFunc);
		void run();
		void trigger(std::string_view functionName);
	protected:
		v8::Isolate* _isolate;
		std::string _fileName;
		std::string _source;
		v8::UniquePersistent<v8::Script> _script;
		std::unique_ptr<v8pp::context> _context{ nullptr };
	};
}