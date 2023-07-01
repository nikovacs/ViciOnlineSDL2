#include "ClientScriptLoader.hpp"
#include <string_view>

void JS::ClientScriptLoader::loadScript(std::string_view fileName) {
	
}

void JS::ClientScriptLoader::unloadScript(std::string_view fileName) {
	_scripts.erase(fileName.data());
}