#include <map>
#include <memory>
#include <v8.h>
#include <libplatform/libplatform.h>
#include <v8pp/class.hpp>
#include <iostream>
#include <stdexcept>
#include "ViciCore/include/JSRuntime/ScriptLoader.hpp"

using namespace std::literals;

JS::ScriptLoader *JS::ScriptLoader::instance = nullptr;

JS::ScriptLoader::ScriptLoader()
{
    _platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(_platform.get());
    v8::V8::Initialize();

    createParams.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

    _isolate = v8::Isolate::New(createParams);
    if (_isolate == nullptr)
    {
        throw std::runtime_error("Failed to create isolate");
    }

    instance = this;
}

JS::ScriptLoader::~ScriptLoader()
{
    v8pp::cleanup(_isolate);
    _isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::DisposePlatform();
    delete createParams.array_buffer_allocator;
}

v8::Isolate *JS::ScriptLoader::getIsolate()
{
    return _isolate;
}