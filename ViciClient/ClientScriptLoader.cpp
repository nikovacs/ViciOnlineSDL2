#include "ClientScriptLoader.hpp"
#include "NetworkAsset.hpp"
#include <string_view>
#include <string>
#include <set>

#include <iostream>

JS::ClientScriptLoader::ClientScriptLoader() : _scriptsInProgress{}, _scripts{} {}

void JS::ClientScriptLoader::update() {
	attemptResolveInProgress();
	for (auto& script : _scripts) {
		script.second->getValue()->trigger("onUpdate");
	}
}

void JS::ClientScriptLoader::attemptResolveInProgress() {
	for (auto& script : _scriptsInProgress) {
		auto scriptPtr = script.second->getValue();
		if (scriptPtr) {
			_scripts[script.first] = std::move(script.second);
			_scriptsInProgress.erase(script.first);
			scriptPtr->trigger("onLoad");
		}
	}
}

void JS::ClientScriptLoader::loadScript(std::string_view fileName) {
	_scriptsInProgress.emplace(fileName, std::make_unique<Networking::NetworkAsset<Script>>(fileName));
}

void JS::ClientScriptLoader::unloadScript(std::string_view fileName) {
	_scripts.erase(fileName.data());
}

void JS::ClientScriptLoader::trigger(std::string_view functionName, std::string_view fileName) {
	if (fileName.empty()) {
		for (auto& script : _scripts) {
			auto scriptPtr = script.second->getValue();
			if (scriptPtr) {
				scriptPtr->trigger(functionName);
			}
		}
	}
	else {
		if (_scripts.count(fileName.data()) > 0) {
			auto scriptPtr = _scripts[fileName.data()]->getValue();
			if (scriptPtr) {
				scriptPtr->trigger(functionName);
			}
		}
	}
}