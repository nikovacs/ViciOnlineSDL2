#pragma once
#include <string_view>
#include <string>
#include <v8.h>
#include <v8pp/context.hpp>
#include <memory>
#include <map>
#include "ViciCore/include/JSRuntime/Script.hpp"

using namespace std::literals;

namespace JS {
	class ScriptLoader {
	public:
		ScriptLoader();
		virtual ~ScriptLoader();
		virtual void loadScript(std::string_view fileName) = 0;
		virtual void unloadScript(std::string_view fileName) = 0;

		/*template <typename... Args>
		virtual void trigger(std::string_view functionName, std::string_view fileName = ""sv, Args... args) = 0;
		virtual void trigger(std::string_view functionName, std::string_view fileName = ""sv) = 0;*/
		v8::Isolate* getIsolate();
		static ScriptLoader* instance;
	protected:
		virtual void setApiSetupFuncs(v8pp::context* ctx) = 0;

		std::unique_ptr<v8::Platform> _platform;
		v8::Isolate* _isolate;
		v8::Isolate::CreateParams createParams;
	};
}