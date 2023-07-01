#pragma once
#include <string_view>
#include <string>
#include <v8.h>
#include <memory>
#include <map>

namespace JS {
	class Script {
	public:
		Script(v8::Isolate* isolate, std::string_view source);
		void trigger(std::string_view functionName);
		void exposeFunction(std::string_view functionName, v8::FunctionCallback callback);
	protected:
		v8::Isolate* _isolate;
		std::string _fileName;
		v8::UniquePersistent<v8::Script> _script;
		v8::UniquePersistent<v8::Context> _context;
	private:
		std::map<std::string, v8::UniquePersistent<v8::FunctionTemplate>> _functionTemplates;
	};
}