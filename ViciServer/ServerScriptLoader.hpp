#pragma once
#include "../ViciEngine/ScriptLoader.hpp"
#include <memory>
#include <map>
#include <string>
#include <string_view>
#include <mutex>
#include <set>

namespace JS {
	class ServerScriptLoader : public ScriptLoader {
	public:
		ServerScriptLoader();
		virtual ~ServerScriptLoader() override;
		virtual void loadScript(std::string_view fileName) override;
		virtual void unloadScript(std::string_view fileName) override;
		virtual void trigger(std::string_view functionName, std::string_view fileName = ""sv) override;
		void update();
		void loadScriptForPlayer(int32_t playerId, std::string_view fileName);
		void unloadScriptForPlayer(int32_t playerId, std::string_view fileName);
		void triggerForPlayer(int32_t playerId, std::string_view functionName, std::string_view fileName = ""sv);
		void onPlayerDisconnect(int32_t playerId);
	private:
		std::map<std::string, std::unique_ptr<JS::Script>> _globalScripts{};
		std::map<int32_t, std::map<std::string, std::unique_ptr<JS::Script>>> _playerScripts{};
		std::set<uint32_t> _playerIdsToRemove{};
		std::recursive_mutex _playersToRemoveMutex{};
	};
}