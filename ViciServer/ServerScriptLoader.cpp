#include "ServerScriptLoader.hpp"
#include "AssetBroker.hpp"
#include <v8pp/module.hpp>
#include <v8pp/class.hpp>
#include "DbJSWrapper.hpp"
#include "ViciServer.hpp"

namespace JS {
	ServerScriptLoader::ServerScriptLoader() {};

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
		std::string fileNameStr{ preProcessScriptFilename(fileName) };
		std::string contents{ Networking::AssetBroker::getFile(fileNameStr)};
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

	void ServerScriptLoader::trigger(std::string_view functionName, std::string_view fileName) {
		if (fileName.empty()) {
			for (auto& [fileName, script] : _globalScripts) {
				script->trigger(functionName);
			}
		}
		else {
			if (_globalScripts.contains(fileName.data())) {
				_globalScripts.at(fileName.data())->trigger(functionName);
			}
		}
	};

	void ServerScriptLoader::update() {
		{
			nlohmann::json& serverOptions = ViciServer::instance->getServerOptions();
			std::string rootScript{ serverOptions["rootScript"] };
			std::lock_guard lock{ _playersNeedingRootScriptMutex };
			for (uint32_t playerId : _playersNeedingRootScript) {
				std::cout << "Loading root script for player " << playerId << std::endl;
				loadScriptForPlayer(playerId, rootScript);
			}
			_playersNeedingRootScript.clear();
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

	void ServerScriptLoader::loadRootScriptForPlayer(int32_t playerId) {
		nlohmann::json& serverOptions = ViciServer::instance->getServerOptions();
		
		std::lock_guard lock{ _playersNeedingRootScriptMutex };
		_playersNeedingRootScript.emplace_back(playerId);
	}

	void ServerScriptLoader::loadScriptForPlayer(int32_t playerId, std::string_view fileName) {
		std::string fileNameStr { preProcessScriptFilename(fileName) };
		std::string contents{ Networking::AssetBroker::getFile(fileNameStr) };
		if (contents.empty()) return;

		_playerScripts[playerId].emplace(fileName, std::make_unique<Script>(getIsolate(), contents));
		Script* script = _playerScripts.at(playerId).at(fileName.data()).get();
		script->initialize([this](v8pp::context* ctx) { setApiSetupFuncs(ctx); });
		script->run();
		script->trigger("onLoad");
	};

	void ServerScriptLoader::unloadScriptForPlayer(int32_t playerId, std::string_view fileName) {
		if (!_playerScripts.contains(playerId)) return;
		if (!_playerScripts.at(playerId).contains(fileName.data())) return;
		_playerScripts.at(playerId).at(fileName.data())->trigger("onUnload");
		_playerScripts.at(playerId).erase(fileName.data());
	};

	void ServerScriptLoader::triggerForPlayer(int32_t playerId, std::string_view functionName, std::string_view fileName) {
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

	void ServerScriptLoader::onPlayerDisconnect(int32_t playerId) {
		std::lock_guard lock{ _playersToRemoveMutex };
		_playerIdsToRemove.insert(playerId);
	};

	void ServerScriptLoader::setApiSetupFuncs(v8pp::context* ctx) {
		std::cout << "Setting up server API" << std::endl;
		setupDatabaseApi(ctx);
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