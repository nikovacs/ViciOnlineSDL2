#include "Script.hpp"
#include <string_view>
#include <string>
#include <v8.h>
#include <v8pp/module.hpp>
#include <iostream>
#include <functional>
#include <v8pp/class.hpp>

JS::Script::Script(v8::Isolate* isolate, std::string_view source) : _isolate{ isolate }, _source{ source } {}

JS::Script::~Script() {}

void JS::Script::initialize(std::function<void(v8pp::context* ctx)> apiSetupFunc) {
    v8::HandleScope handleScope{ _isolate };
    _context = std::make_unique<v8pp::context>(_isolate);

    v8::Local<v8::String> sourceCode{ v8::String::NewFromUtf8(_isolate, _source.data(), v8::NewStringType::kNormal, static_cast<int>(_source.size())).ToLocalChecked() };
    v8::Local<v8::Script> script{ v8::Script::Compile(_context->impl(), sourceCode).ToLocalChecked() };
    _script.Reset(_isolate, script);

    apiSetupFunc(_context.get());
}

void JS::Script::run() {
    v8::Isolate::Scope isolateScope{ _isolate };
    v8::HandleScope handleScope{ _isolate };
    v8::Context::Scope contextScope{ _context->impl() };

    _script.Get(_isolate)->Run(_context->impl()).ToLocalChecked();
}

