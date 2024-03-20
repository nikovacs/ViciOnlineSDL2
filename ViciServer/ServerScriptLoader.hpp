#pragma once
#include "../ViciEngine/ScriptLoader.hpp"
#include <memory>
#include <map>
#include <string>
#include <string_view>
#include <mutex>
#include <vector>
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

		/*
		* This method is called from the Udp thread, so the script must be
		* loaded later on the main thread.
		*
		* Stored in a thread-safe vector now for safekeeping
		*
		* @param playerId The connection ID of the player
		*/
		void loadRootScriptForPlayer(int32_t playerId);
		void loadScriptForPlayer(int32_t playerId, std::string_view fileName);
		void unloadScriptForPlayer(int32_t playerId, std::string_view fileName);
		void triggerForPlayer(int32_t playerId, std::string_view functionName, std::string_view fileName = ""sv);
		void onPlayerDisconnect(int32_t playerId);
	private:
		/*
		* Preprocess the script filename to include the ss_ prefix
		* ss_ is an abbreviation for server script
		*/
		inline static std::string preProcessScriptFilename(std::string_view fileName) {
			return "ss_" + std::string(fileName);
		}
		virtual void setApiSetupFuncs(v8pp::context* ctx) override;
		void setupDatabaseApi(v8pp::context* ctx);
		std::map<std::string, std::unique_ptr<JS::Script>> _globalScripts{};
		std::map<int32_t, std::map<std::string, std::unique_ptr<JS::Script>>> _playerScripts{};
		std::set<uint32_t> _playerIdsToRemove{};
		std::recursive_mutex _playersToRemoveMutex{};

		std::vector<uint32_t> _playersNeedingRootScript{};
		std::recursive_mutex _playersNeedingRootScriptMutex{};
	};
}