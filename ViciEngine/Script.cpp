#include "Script.hpp"
#include <string_view>
#include <string>
#include <v8.h>

void JS::Script::trigger(std::string_view functionName) {
    v8::Context::Scope context_scope(_context);
    v8::HandleScope handle_scope(_isolate);
    
    v8::Local<v8::Value> func_value;
	v8::Local<v8::String> foo_property = v8::String::NewFromUtf8(_isolate, functionName.data()).ToLocalChecked();
    if (_context->Global()->Get(_context, foo_property).ToLocal(&func_value) && func_value->IsFunction()) {
        v8::Local<v8::Function> func_function = v8::Local<v8::Function>::Cast(func_value);

        // Call the function with zero arguments
        v8::Local<v8::Value> result = func_function->Call(_context, _context->Global(), 0, nullptr).ToLocalChecked();
    }
}