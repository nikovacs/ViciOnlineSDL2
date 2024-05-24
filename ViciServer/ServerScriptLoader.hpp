#pragma once
#include "../ViciEngine/ScriptLoader.hpp"
#include <memory>
#include <map>
#include <string>
#include <string_view>
#include <mutex>
#include <set>
#include <unordered_set>
#include <iostream>
#include <v8pp/class.hpp>

namespace JS {
	class ServerScriptLoader : public ScriptLoader {
	public:
		ServerScriptLoader();
		virtual ~ServerScriptLoader() override;
		virtual void loadScript(std::string_view fileName) override;
		virtual void unloadScript(std::string_view fileName) override;
		template <typename... Args>
		void trigger(std::string_view functionName, std::string_view fileName, Args&&... args) {
			if (fileName.empty()) {
				for (auto& [fileName, script] : _globalScripts) {
					script->trigger(functionName, std::forward<Args>(args)...);
				}
			}
			else {
				if (_globalScripts.contains(fileName.data())) {
					_globalScripts.at(fileName.data())->trigger(functionName, std::forward<Args>(args)...);
				}
			}
		}
		void update();
		void loadScriptForPlayer(uint32_t playerId, std::string_view fileName);
		void unloadScriptForPlayer(uint32_t playerId, std::string_view fileName);
		void triggerForPlayer(uint32_t playerId, std::string_view functionName, std::string_view fileName = ""sv);
		void onPlayerDisconnect(uint32_t playerId);
		void onPlayerConnect(uint32_t playerId);
	private:
		virtual void setApiSetupFuncs(v8pp::context* ctx) override;
		void setupStandardFuncs(v8pp::context* ctx);
		void setupServerPlayerApi(v8pp::context* ctx);
		void setupDatabaseApi(v8pp::context* ctx);
		std::map<std::string, std::unique_ptr<JS::Script>> _globalScripts{};
		std::map<uint32_t, std::map<std::string, std::unique_ptr<JS::Script>>> _playerScripts{};
		std::set<uint32_t> _playerIdsToRemove{};
		std::recursive_mutex _playersToRemoveMutex{};

		std::mutex _playerIdsConnectedMutex{};
		std::unordered_set<uint32_t> _playerIdsConnected{};
	};
}