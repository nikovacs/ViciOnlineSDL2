#include "ClientScriptLoader.hpp"
#include "NetworkAsset.hpp"
#include <string_view>
#include <string>
#include <memory>
#include <set>
#include <vector>
#include <v8.h>
#include <v8pp/module.hpp>
#include "KeyboardInputHandler.hpp"

JS::ClientScriptLoader::ClientScriptLoader() {}

JS::ClientScriptLoader::~ClientScriptLoader() {
	for (auto& script : _scripts) {
		auto* scriptPtr = script.second->getValue();
		if (scriptPtr) {
			scriptPtr->trigger("onUnload");
		}
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

	auto x = [this]()->void {
		v8pp::module test{ _isolate };
		test.property("dir",
			[this]() -> int {
				return _clientPlayer->getDir();
			},
			[this](int value) {
				_clientPlayer->setDir(value);
			}
			);

		_isolate->GetCurrentContext()->Global()->Set(_isolate->GetCurrentContext(), v8::String::NewFromUtf8(_isolate, "clientPlayer").ToLocalChecked(), test.new_instance()).FromJust();
	};
	
	std::vector<std::string> toRemove{};
	for (auto& script : _scriptsInProgress) {
		auto scriptPtr = script.second->getValue();
		if (scriptPtr) {
			_scripts[script.first] = std::move(script.second);
			toRemove.push_back(script.first);
			scriptPtr->initialize([this]()->void { setApiSetupFuncs(); });
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
		auto* scriptPtr = _scripts.at(fileName.data())->getValue();
		if (scriptPtr) {
			scriptPtr->trigger("onUnload");
		}
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

void JS::ClientScriptLoader::setClientPlayer(Entities::ClientPlayer* pl) {
	_clientPlayer = pl;
}

void JS::ClientScriptLoader::setApiSetupFuncs() {
	if (!_clientPlayer) {
		throw std::runtime_error{ "ClientScriptLoader::getApiSetupFuncs() called before ClientScriptLoader::setClientPlayer()!" };
	}

	static v8pp::module clientPlayer{ _isolate };
	clientPlayer
		.property(
			"dir",
			[this]() -> int { return _clientPlayer->getDir(); },
			[this](int dir) { _clientPlayer->setDir(dir); }
			)
		.property(
			"ani",
			[this]() -> std::string { return _clientPlayer->getAni(); },
			[this](std::string ani) { _clientPlayer->setAniHard(ani); }
			)
		.property(
			"width",
			[this]() -> int { return _clientPlayer->getWidth(); },
			[this](int width) { _clientPlayer->setWidth(width); }
			)
		.property(
			"height",
			[this]() -> int { return _clientPlayer->getHeight(); },
			[this](int height) { _clientPlayer->setHeight(height); }
			)
		.property(
			"pos",
			[this]() -> std::vector<int> { return { _clientPlayer->getX(), _clientPlayer->getY() }; },
			[this](std::vector<int> pos) { _clientPlayer->setPosition(pos[0], pos[1]); }
			)
		;
	_isolate->GetCurrentContext()->Global()->Set(_isolate->GetCurrentContext(), v8pp::to_v8(_isolate, "clientPlayer"), clientPlayer.new_instance());

	v8::Local<v8::Function> isKeyDownFunc = v8pp::wrap_function(_isolate, "isKeyDown", &Handlers::KeyboardInputHandler::isKeyDown);
	_isolate->GetCurrentContext()->Global()->Set(_isolate->GetCurrentContext(), v8pp::to_v8(_isolate, "isKeyDown"), isKeyDownFunc);

}