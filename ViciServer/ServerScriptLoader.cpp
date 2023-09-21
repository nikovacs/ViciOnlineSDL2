#include "ServerScriptLoader.hpp"
#include "AssetBroker.hpp"

namespace JS {
	ServerScriptLoader::ServerScriptLoader() {};

	ServerScriptLoader::~ServerScriptLoader() {
		for (auto& [fileName, script] : _globalScripts) {
			script.trigger("onUnload");
		}

		for (auto& [playerId, scripts] : _playerScripts) {
			for (auto& [fileName, script] : scripts) {
				script.trigger("onUnload");
			}
		}
	};

	void ServerScriptLoader::loadScript(std::string_view fileName) {
		std::string contents{ Networking::AssetBroker::readFile(fileName) };
		if (contents.empty()) return;

		_globalScripts.emplace(fileName, Script{ getIsolate(), contents });
	};

	void ServerScriptLoader::unloadScript(std::string_view fileName) {
		if (_globalScripts.contains(fileName.data())) {
			_globalScripts.at(fileName.data()).trigger("onUnload");
			_globalScripts.erase(fileName.data());
		}
	};

	void ServerScriptLoader::trigger(std::string_view functionName, std::string_view fileName) {
		if (fileName.empty()) {
			for (auto& [fileName, script] : _globalScripts) {
				script.trigger(functionName);
			}
		}
		else {
			if (_globalScripts.contains(fileName.data())) {
				_globalScripts.at(fileName.data()).trigger(functionName);
			}
		}
	};

	void ServerScriptLoader::update() {
		for (auto& [fileName, script] : _globalScripts) {
			script.trigger("onUpdate");
		}

		for (auto& [playerId, scripts] : _playerScripts) {
			for (auto& [fileName, script] : scripts) {
				script.trigger("onUpdate");
			}
		}
	};

	void ServerScriptLoader::loadScriptForPlayer(int32_t playerId, std::string_view fileName) {
		std::string contents{ Networking::AssetBroker::readFile(fileName) };
		if (contents.empty()) return;

		_playerScripts[playerId].emplace(fileName, Script{ getIsolate(), contents });
	};

	void ServerScriptLoader::unloadScriptForPlayer(int32_t playerId, std::string_view fileName) {
		if (!_playerScripts.contains(playerId)) return;
		if (!_playerScripts.at(playerId).contains(fileName.data())) return;
		_playerScripts.at(playerId).at(fileName.data()).trigger("onUnload");
		_playerScripts.at(playerId).erase(fileName.data());
	};

	void ServerScriptLoader::triggerForPlayer(int32_t playerId, std::string_view functionName, std::string_view fileName) {
		if (!_playerScripts.contains(playerId)) return;
		if (fileName.empty()) {
			for (auto& [fileName, script] : _playerScripts.at(playerId)) {
				script.trigger(functionName);
			}
		}
		else {
			if (_playerScripts.at(playerId).contains(fileName.data())) {
				_playerScripts.at(playerId).at(fileName.data()).trigger(functionName);
			}
		}
	};

	void ServerScriptLoader::onPlayerDisconnect(int32_t playerId) {
		if (!_playerScripts.contains(playerId)) return;
		for (auto& [fileName, script] : _playerScripts.at(playerId)) {
			script.trigger("onUnload");
		}
		_playerScripts.erase(playerId);
	};
}