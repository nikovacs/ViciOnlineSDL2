#include "ClientScriptLoader.hpp"
#include "NetworkAsset.hpp"
#include <string_view>
#include <string>
#include <memory>
#include <set>
#include <vector>

JS::ClientScriptLoader::ClientScriptLoader() {}

JS::ClientScriptLoader::~ClientScriptLoader() {
	for (auto& script : _scripts) {
		script.second->getValue()->trigger("onUnload");
	}
}

void JS::ClientScriptLoader::update() {
	attemptResolveInProgress();
	for (auto& script : _scripts) {
		script.second->getValue()->trigger("onUpdate");
	}
}

void JS::ClientScriptLoader::attemptResolveInProgress() {
	if (_scriptsInProgress.size() == 0) return;
	
	std::vector<std::string> toRemove{};
	for (auto& script : _scriptsInProgress) {
		auto scriptPtr = script.second->getValue();
		if (scriptPtr) {
			_scripts[script.first] = std::move(script.second);
			toRemove.push_back(script.first);
			scriptPtr->run();
			scriptPtr->trigger("onLoad");
		}
	}
	for (auto& script : toRemove) {
		_scriptsInProgress.erase(script);
	}
}

void JS::ClientScriptLoader::loadScript(std::string_view fileName) {
	_scriptsInProgress.emplace(fileName, std::make_unique<Networking::NetworkAsset<JS::Script>>(fileName));
}

void JS::ClientScriptLoader::unloadScript(std::string_view fileName) {
	if (_scripts.contains(fileName.data())) {
		_scripts.at(filename.data())->trigger("onUnload"));
	}
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