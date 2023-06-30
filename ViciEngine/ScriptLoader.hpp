#pragma once
#include <string_view>
#include <string>
#include <v8.h>
#include <memory>
#include <map>
#include "Script.hpp"

using namespace std::literals;

namespace JS {
	class ScriptLoader {
	public:
		ScriptLoader();
		virtual ~ScriptLoader();
		virtual void loadScript(std::string_view fileName) = 0;
		virtual void unloadScript(std::string_view fileName) = 0;
		void trigger(std::string_view functionName, std::string_view fileName = ""sv);
	protected:
		std::map<std::string, std::unique_ptr<Script>> _scripts;
		std::unique_ptr<v8::Platform> _platform;
		v8::Isolate* _isolate;
	};
}