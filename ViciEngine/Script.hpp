#pragma once
#include <string_view>
#include <string>
#include <v8.h>
#include <memory>

namespace JS {
	class Script {
	public:
		Script(v8::Isolate* isolate, std::string_view source);
		void trigger(std::string_view functionName);
	protected:
		v8::Isolate* _isolate;
		std::string _fileName;
		v8::UniquePersistent<v8::Script> _script;
		v8::UniquePersistent<v8::Context> _context;
	};
}