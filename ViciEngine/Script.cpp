#include "Script.hpp"
#include "StandardFuncs.hpp"
#include <string_view>
#include <string>
#include <v8.h>

JS::Script::Script(v8::Isolate* isolate, std::string_view source) : _functionTemplates{}, _isolate { isolate }, _context{ isolate, v8::Context::New(isolate) } {
	v8::Isolate::Scope isolateScope{ isolate };
	v8::HandleScope handleScope{ isolate };
	v8::Local<v8::Context> context{ _context.Get(isolate) };
	v8::Context::Scope contextScope{ context };
	v8::Local<v8::String> sourceCode{ v8::String::NewFromUtf8(isolate, source.data(), v8::NewStringType::kNormal, static_cast<int>(source.size())).ToLocalChecked() };
	v8::Local<v8::Script> script{ v8::Script::Compile(context, sourceCode).ToLocalChecked() };
	_script.Reset(isolate, script);

	exposeFunction("print", JS::printCallback);
    
    script->Run(context);
}


void JS::Script::trigger(std::string_view functionName) {
    v8::Local<v8::Context> context = _context.Get(_isolate);
    
    v8::Context::Scope context_scope(context);
    v8::HandleScope handle_scope(_isolate);
    
    v8::Local<v8::Value> func_value;
	v8::Local<v8::String> foo_property = v8::String::NewFromUtf8(_isolate, functionName.data()).ToLocalChecked();
    if (context->Global()->Get(context, foo_property).ToLocal(&func_value) && func_value->IsFunction()) {
        v8::Local<v8::Function> func_function = v8::Local<v8::Function>::Cast(func_value);

        // Call the function with zero arguments
        v8::Local<v8::Value> result = func_function->Call(context, context->Global(), 0, nullptr).ToLocalChecked();
    }
}

void JS::Script::exposeFunction(std::string_view functionName, v8::FunctionCallback callback) {
	_functionTemplates.emplace(functionName, v8::UniquePersistent<v8::FunctionTemplate>{ _isolate, v8::FunctionTemplate::New(_isolate, callback) });
	_functionTemplates[functionName.data()].Get(_isolate)->SetClassName(v8::String::NewFromUtf8(_isolate, functionName.data()).ToLocalChecked());
	_context.Get(_isolate)->Global()->Set(_context.Get(_isolate), v8::String::NewFromUtf8(_isolate, functionName.data()).ToLocalChecked(), _functionTemplates[functionName.data()].Get(_isolate)->GetFunction(_context.Get(_isolate)).ToLocalChecked()).FromJust();
}