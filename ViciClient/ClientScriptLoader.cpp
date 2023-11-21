#include "ClientScriptLoader.hpp"
#include "NetworkAsset.hpp"
#include <string_view>
#include <string>
#include <memory>
#include <set>
#include <vector>
#include <v8pp/class.hpp>
#include <v8pp/context.hpp>
#include <v8pp/module.hpp>
#include "KeyboardInputHandler.hpp"
#include "NetworkedPlayer.hpp"
#include "ClientPlayerManager.hpp"
#include "NetworkedPlayerJSWrapper.hpp"

JS::ClientScriptLoader::ClientScriptLoader() {}

JS::ClientScriptLoader::~ClientScriptLoader() {
	for (auto& script : _scripts) {
		auto* scriptPtr = script.second->getValue();
		if (scriptPtr) {
			scriptPtr->trigger("onUnload");
		}
		// it is important that the scripts in these unique ptrs are released immediately
		// the shut down order of v8 is important
		script.second.release();
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
			scriptPtr->initialize([this](v8pp::context* ctx)->void { setApiSetupFuncs(ctx); });
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

void JS::ClientScriptLoader::setApiSetupFuncs(v8pp::context* ctx) {
	if (!_clientPlayer) {
		throw std::runtime_error{ "ClientScriptLoader::getApiSetupFuncs() called before ClientScriptLoader::setClientPlayer()!" };
	}

	exposeClientPlayer(ctx);
	exposeKeyboardHandler(ctx);
	exposeLocalAttrs(ctx);
	exposeNetworkedPlayerClass(ctx);
	exposeNetworkPlayerManagerFunctions(ctx);
}

void JS::ClientScriptLoader::exposeClientPlayer(v8pp::context* ctx) {
	v8pp::module clientPlayer{ _isolate };
	clientPlayer
		.property(
			"dir",
			[this]() -> int { return _clientPlayer->getDir(); },
			[this](int dir) { _clientPlayer->setDir(dir); }
			)
		.property(
			"ani",
			[this]() -> std::string_view { return _clientPlayer->getAni(); },
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
	ctx->module("clientPlayer", clientPlayer);
}

void JS::ClientScriptLoader::exposeKeyboardHandler(v8pp::context* ctx) {
	v8::HandleScope scope{ _isolate };

	v8::Local<v8::Function> isKeyDownFunc = v8pp::wrap_function(_isolate, "isKeyDown", &Handlers::KeyboardInputHandler::isKeyDown);
	_isolate->GetCurrentContext()->Global()->Set(_isolate->GetCurrentContext(), v8pp::to_v8(_isolate, "isKeyDown"), isKeyDownFunc);
}

void JS::ClientScriptLoader::exposeLocalAttrs(v8pp::context* ctx) {
	v8::HandleScope scope{ _isolate };

	static nlohmann::json localAttrs{};
	v8pp::module localAttrsModule{ _isolate };
	localAttrsModule
		.function("setInt", [](std::string key, int64_t value)->void { localAttrs[key] = value; })
		.function("getInt", [](std::string key)->int64_t { return localAttrs[key]; })
		.function("setFloat", [](std::string key, double value)->void { localAttrs[key] = value; })
		.function("getFloat", [](std::string key)->double { return localAttrs[key]; })
		.function("setString", [](std::string key, std::string value)->void { localAttrs[key] = value; })
		.function("getString", [](std::string key)->std::string { return localAttrs[key]; })
		.function("setBool", [](std::string key, bool value)->void { localAttrs[key] = value; })
		.function("getBool", [](std::string key)->bool { return localAttrs[key]; })
		.function("getType",
			[](std::string key)->std::string {
				if (!localAttrs.contains(key)) return "undefined";
				if (localAttrs[key].is_number_integer()) return "int";
				if (localAttrs[key].is_number_float()) return "float";
				if (localAttrs[key].is_string()) return "string";
				if (localAttrs[key].is_boolean()) return "bool";
				return "undefined";
			}
	);
	ctx->module("localAttrs", localAttrsModule);

	static std::string localAttrsProxyScript{ R"(
		const __VICI__INTERNAL__localAttrsHandler = {
			get: function(target, prop, receiver) {
				let type = __VICI__localAttrsModule.getType(prop);
				if (type === "int") {
					return __VICI__localAttrsModule.getInt(prop);
				} else if (type === "float") {
					return __VICI__localAttrsModule.getFloat(prop);
				} else if (type === "string") {
					return __VICI__localAttrsModule.getString(prop);
				} else if (type === "bool") {
					return __VICI__localAttrsModule.getBool(prop);
				} else {
					return undefined;
				}
			},
			set: function(target, prop, value) {
				if (Number.isInteger(value)) {
					__VICI__localAttrsModule.setInt(prop, value);
				} else if (typeof value === "number") {
					__VICI__localAttrsModule.setFloat(prop, value);
				} else if (typeof value === "string") {
					__VICI__localAttrsModule.setString(prop, value);
				} else if (typeof value === "boolean") {
					__VICI__localAttrsModule.setBool(prop, value);
				}
			}
		}
		const localAttrs = new Proxy({}, __VICI__INTERNAL__localAttrsHandler);
	)" };
	v8::Local<v8::String> localAttrsProxyScriptString = v8pp::to_v8(_isolate, localAttrsProxyScript);
	v8::Local<v8::Script> localAttrsProxyScriptCompiled = v8::Script::Compile(_isolate->GetCurrentContext(), localAttrsProxyScriptString).ToLocalChecked();
	localAttrsProxyScriptCompiled->Run(_isolate->GetCurrentContext()).ToLocalChecked();
}

void JS::ClientScriptLoader::exposeNetworkedPlayerClass(v8pp::context* ctx) {
	/*static v8pp::class_<Entities::NetworkedPlayer> networkedPlayerClass{ _isolate };
	networkedPlayerClass
		.auto_wrap_objects(true)
		.property("dir", &Entities::NetworkedPlayer::getDir)
		.property("ani", &Entities::NetworkedPlayer::getAni)
		.property("width", &Entities::NetworkedPlayer::getWidth)
		.property("height", &Entities::NetworkedPlayer::getHeight)
		;
	ctx->class_("networkedPlayer", networkedPlayerClass);*/
	static v8pp::class_<JS::NetworkedPlayerJSWrapper> networkedPlayerClass{ _isolate };
	networkedPlayerClass
		.auto_wrap_objects(true)
		.property("username", &JS::NetworkedPlayerJSWrapper::getUsername)
		.property("dir", &JS::NetworkedPlayerJSWrapper::getDir)
		.property("ani", &JS::NetworkedPlayerJSWrapper::getAni)
		.property("width", &JS::NetworkedPlayerJSWrapper::getWidth)
		.property("height", &JS::NetworkedPlayerJSWrapper::getHeight)
		;
	ctx->class_("networkedPlayer", networkedPlayerClass);
}

void JS::ClientScriptLoader::exposeNetworkPlayerManagerFunctions(v8pp::context* ctx) {
	ctx->function("getPlayer", [this](std::string_view username)->JS::NetworkedPlayerJSWrapper {
		auto* pl{ Networking::ClientPlayerManager::getPlayer(username) };
		if (pl) {
			return JS::NetworkedPlayerJSWrapper{ pl };
		}
		return JS::NetworkedPlayerJSWrapper{};
	});
}