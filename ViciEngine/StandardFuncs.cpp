#include "StandardFuncs.hpp"
#include <iostream>
#include <v8.h>

namespace JS {
	void printCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
		v8::Isolate* isolate = args.GetIsolate();
		v8::HandleScope scope(isolate);
		
		for (int i = 0; i < args.Length(); i++) {
			v8::String::Utf8Value str(isolate, args[i]);
			std::cout << *str << " ";
		}
	}
}