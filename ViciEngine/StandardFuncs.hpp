#pragma once
#include <v8.h>

namespace JS {
	void printCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
}