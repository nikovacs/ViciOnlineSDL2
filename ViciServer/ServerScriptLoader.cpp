#include "ServerScriptLoader.hpp"
#include "AssetBroker.hpp"
#include <v8pp/module.hpp>
#include <v8pp/class.hpp>
#include "DbJSWrapper.hpp"
#include "ViciServer.hpp"
#include "ServerPlayerManager.hpp"
#include "ServerPlayerJSWrapper.hpp"

namespace JS {
	ServerScriptLoader::ServerScriptLoader() {
		nlohmann::json& serverOpts = ViciServer::instance->getServerOptions();
		std::string rootScript = serverOpts["rootScript"];
		loadScript(rootScript);
	};

	ServerScriptLoader::~ServerScriptLoader() {
		for (auto& [fileName, script] : _globalScripts) {
			script->trigger("onUnload");
		}

		for (auto& [playerId, scripts] : _playerScripts) {
			for (auto& [fileName, script] : scripts) {
				script->trigger("onUnload");
			}
		}
	};

	void ServerScriptLoader::loadScript(std::string_view fileName) {
		std::string contents{ Networking::AssetBroker::getFile(fileName)};
		if (contents.empty()) return;
		_globalScripts.emplace(fileName, std::make_unique<Script>(getIsolate(), contents));
		Script* script = _globalScripts.at(fileName.data()).get();
		script->initialize([this](v8pp::context* ctx) { setApiSetupFuncs(ctx); });
		script->run();
		script->trigger("onLoad");
	};

	void ServerScriptLoader::unloadScript(std::string_view fileName) {
		if (_globalScripts.contains(fileName.data())) {
			_globalScripts.at(fileName.data())->trigger("onUnload");
			_globalScripts.erase(fileName.data());
		}
	};

	void ServerScriptLoader::update() {
		nlohmann::json& serverOpts = ViciServer::instance->getServerOptions();
		if (serverOpts.contains("rootScript")) {
			std::string rootScript{ serverOpts["rootScript"] };
			std::lock_guard<std::mutex> lock{ _playerIdsConnectedMutex };
			for (uint32_t playerId : _playerIdsConnected) {
				Entities::ServerPlayer* pl = Networking::ServerPlayerManager::getPlayer(playerId);
				if (pl == nullptr) continue;
				trigger("onPlayerLogin", rootScript, JS::ServerPlayerJSWrapper{ pl });
			}
			_playerIdsConnected.clear();
		}

		for (auto& [fileName, script] : _globalScripts) {
			script->trigger("onUpdate");
		}

		for (auto& [playerId, scripts] : _playerScripts) {
			for (auto& [fileName, script] : scripts) {
				script->trigger("onUpdate");
			}
		}

		// remove players that have disconnected on the main thread
		std::vector<uint32_t> playerIdsRemoved{};
		std::lock_guard lock{ _playersToRemoveMutex };
		for (uint32_t playerId : _playerIdsToRemove) {
			if (!_playerScripts.contains(playerId)) return;
			for (auto& [fileName, script] : _playerScripts.at(playerId)) {
				script->trigger("onUnload");
			}
			_playerScripts.erase(playerId);
			playerIdsRemoved.push_back(playerId);
		}

		for (uint32_t playerId : playerIdsRemoved) {
			_playerIdsToRemove.erase(playerId);
		}
	};

	void ServerScriptLoader::loadScriptForPlayer(uint32_t playerId, std::string_view fileName) {
		std::string contents{ Networking::AssetBroker::getFile(fileName) };
		if (contents.empty()) return;

		_playerScripts[playerId].emplace(fileName, std::make_unique<Script>(getIsolate(), contents));
		Script* script = _playerScripts.at(playerId).at(fileName.data()).get();
		script->initialize([this](v8pp::context* ctx) { setApiSetupFuncs(ctx); });
		script->run();
		script->trigger("onLoad");
	};

	void ServerScriptLoader::unloadScriptForPlayer(uint32_t playerId, std::string_view fileName) {
		if (!_playerScripts.contains(playerId)) return;
		if (!_playerScripts.at(playerId).contains(fileName.data())) return;
		_playerScripts.at(playerId).at(fileName.data())->trigger("onUnload");
		_playerScripts.at(playerId).erase(fileName.data());
	};

	void ServerScriptLoader::triggerForPlayer(uint32_t playerId, std::string_view functionName, std::string_view fileName) {
		if (!_playerScripts.contains(playerId)) return;
		if (fileName.empty()) {
			for (auto& [fileName, script] : _playerScripts.at(playerId)) {
				script->trigger(functionName);
			}
		}
		else {
			if (_playerScripts.at(playerId).contains(fileName.data())) {
				_playerScripts.at(playerId).at(fileName.data())->trigger(functionName);
			}
		}
	};

	void ServerScriptLoader::onPlayerDisconnect(uint32_t playerId) {
		std::lock_guard lock{ _playersToRemoveMutex };
		_playerIdsToRemove.insert(playerId);
	};

	void ServerScriptLoader::onPlayerConnect(uint32_t playerId) {
		std::lock_guard<std::mutex> lock{ _playerIdsConnectedMutex };
		_playerIdsConnected.insert(playerId);
	};

	void ServerScriptLoader::setApiSetupFuncs(v8pp::context* ctx) {
		std::cout << "Setting up server API" << std::endl;
		setupStandardFuncs(ctx);
		setupServerPlayerApi(ctx);
		setupDatabaseApi(ctx);
	}

	void ServerScriptLoader::setupStandardFuncs(v8pp::context* ctx) {
		ctx->function("print", [](const v8::FunctionCallbackInfo<v8::Value>& args) {
			v8::Isolate* isolate = args.GetIsolate();
			v8::HandleScope scope(isolate);

			if (args.Length() < 1) {
				std::cout << std::endl;
				return;
			}
			for (int i = 0; i < args.Length(); i++) {
				v8::String::Utf8Value str(isolate, args[i]);
				std::cout << *str;

				if (i < args.Length() - 1) {
					std::cout << " ";
				}
			}

			std::cout << std::endl;
			});
	}

	void ServerScriptLoader::setupServerPlayerApi(v8pp::context* ctx) {
		static v8pp::class_<JS::ServerPlayerJSWrapper> serverPlayerJSWrapper{ _isolate };
		serverPlayerJSWrapper
			.auto_wrap_objects(true)
			.function("addScript", &JS::ServerPlayerJSWrapper::addScript)
			.function("removeScript", &JS::ServerPlayerJSWrapper::removeScript)
			;

		ctx->class_("ServerPlayer", serverPlayerJSWrapper);
	}

	void ServerScriptLoader::setupDatabaseApi(v8pp::context* ctx) {
		static v8pp::class_<Vici::DbResultsJSWrapper> dbResultsJSWrapper{ _isolate };
		dbResultsJSWrapper
			.auto_wrap_objects(true)
			.function("next", &Vici::DbResultsJSWrapper::next)
			.function("hasNext", &Vici::DbResultsJSWrapper::hasNext)
			.function("isEmpty", &Vici::DbResultsJSWrapper::isEmpty)
			.function("getString", &Vici::DbResultsJSWrapper::getValue<std::string>)
			.function("getSmallInt", &Vici::DbResultsJSWrapper::getValue<int16_t>)
			.function("getInt", &Vici::DbResultsJSWrapper::getValue<int32_t>)
			.function("getBigInt", &Vici::DbResultsJSWrapper::getBigInt)
			.function("getReal", &Vici::DbResultsJSWrapper::getValue<float>)
			.function("getDouble", &Vici::DbResultsJSWrapper::getValue<double>)
			// To add the following two types would require a library like bigdecimal.js
			//.function("getNumeric", &Vici::DbResultsJSWrapper::getNumeric)
			//.function("getDecimal", &Vici::DbResultsJSWrapper::getDecimal)
			.function("getBool", &Vici::DbResultsJSWrapper::getValue<bool>)
			.function("getJson", &Vici::DbResultsJSWrapper::getJson)
			.function("getArrayOfString", &Vici::DbResultsJSWrapper::getArrayOf<std::string>)
			.function("getArrayOfSmallInt", &Vici::DbResultsJSWrapper::getArrayOf<int16_t>)
			.function("getArrayOfInt", &Vici::DbResultsJSWrapper::getArrayOf<int32_t>)
			.function("getArrayOfBigInt", &Vici::DbResultsJSWrapper::getArrayOf<int64_t>)
			.function("getArrayOfReal", &Vici::DbResultsJSWrapper::getArrayOf<float>)
			.function("getArrayOfDouble", &Vici::DbResultsJSWrapper::getArrayOf<double>)
			.function("getArrayOfBool", &Vici::DbResultsJSWrapper::getArrayOf<bool>)
			.function("getArrayOfJson", &Vici::DbResultsJSWrapper::getArrayOf<v8::Local<v8::Value>>)
			;
		ctx->class_("DbResults", dbResultsJSWrapper);

		/*static v8pp::class_<Vici::DbTransactionJSWrapper> dbTransactionJSWrapper{ _isolate };
		dbTransactionJSWrapper
			.auto_wrap_objects(true)
			.function("exec", &Vici::DbTransactionJSWrapper::exec)
			.function("commit", &Vici::DbTransactionJSWrapper::commit)
			;
		ctx->class_("DbTransaction", dbTransactionJSWrapper);*/

		static v8pp::module dbApi{ _isolate };
		dbApi
			.function("exec", [this](std::string sql) { return ViciServer::instance->getDbAsyncQueryRunner().runQuery(sql, _isolate->GetCurrentContext()); })
			//.function("beginTransaction", []() { return dbTransactionJSWrapper.create_object(v8::Isolate::GetCurrent()); })
			;
		ctx->module("db", dbApi);
	}
}