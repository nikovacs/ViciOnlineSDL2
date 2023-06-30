#include "ScriptLoader.hpp"
#include <map>
#include <memory>
#include <v8.h>
#include <libplatform/libplatform.h>

using namespace std::literals;

JS::ScriptLoader::ScriptLoader() {
	_scripts = std::map<std::string, std::unique_ptr<Script>>();
    
    v8::V8::InitializeICUDefaultLocation(nullptr);
    v8::V8::InitializeExternalStartupData(nullptr);
	_platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(_platform.get());
    v8::V8::Initialize();

    v8::Isolate::CreateParams create_params;
    _isolate = v8::Isolate::New(create_params);
}

JS::ScriptLoader::~ScriptLoader() {
    _isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::DisposePlatform();
}

void JS::ScriptLoader::trigger(std::string_view functionName, std::string_view fileName) {
	if (fileName.empty()) {
		for (auto& script : _scripts) {
			script.second->trigger(functionName);
		}
	}
	else {
		_scripts[fileName.data()]->trigger(functionName);
	}
}