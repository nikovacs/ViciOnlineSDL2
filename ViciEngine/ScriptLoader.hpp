#pragma once
#include <string_view>
#include <v8/v8.h>
#include <memory>
#include <map>

class ScriptLoader {
public:
	ScriptLoader();
	virtual ~ScriptLoader();
	void loadScript(std::string_view fileName);
	void unloadScript(std::string_view fileName);
private:
	std::unique_ptr<v8::Platform> _platform;
	std::unique_ptr<v8::Isolate> _isolate;
};