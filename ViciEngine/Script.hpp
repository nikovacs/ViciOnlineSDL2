#pragma once
#include <string_view>
#include <v8/v8.h>
#include <memory>

class Script {
public:
	Script(std::string_view fileName);
	virtual ~Script();
private:
	std::unique_ptr<v8::Context> _context;
	std::unique_ptr<v8::HandleScope> _handleScope;
};