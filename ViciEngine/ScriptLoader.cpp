#include "ScriptLoader.hpp"
#include <map>
#include <memory>
#include <v8.h>
#include <libplatform/libplatform.h>

using namespace std::literals;

JS::ScriptLoader::ScriptLoader() {
	_scripts = std::map<std::string, std::unique_ptr<Script>>();

	_platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(_platform.get());
    v8::V8::Initialize();

    createParams.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

    _isolate = v8::Isolate::New(createParams);
}

JS::ScriptLoader::~ScriptLoader() {
    _isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::DisposePlatform();
	delete createParams.array_buffer_allocator;
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