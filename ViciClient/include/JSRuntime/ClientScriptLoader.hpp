#pragma once

#include <map>
#include <set>
#include <string>
#include <memory>
#include "header_wrappers/v8pp_wrapper.h"
#include "ViciCore/include/JSRuntime/ScriptLoader.hpp"
#include "ViciClient/include/Networking/NetworkAsset.hpp"
#include "ViciClient/include/Entities/ClientPlayer.hpp"

using namespace std::literals;

namespace JS {
	class ClientScriptLoader : public ScriptLoader {
	public:
		ClientScriptLoader();
		virtual ~ClientScriptLoader() override;
		virtual void loadScript(std::string_view fileName) override;
		virtual void unloadScript(std::string_view fileName) override;
		template <typename... Args>
		void trigger(std::string_view functionName, std::string_view fileName, [[maybe_unused]] Args&&... args) {
				// TODO remove [[maybe_unused]] when we support arg passing
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
		void update();
		void setClientPlayer(Entities::ClientPlayer* pl);
		void onLoadScript(std::string_view fileName);
		void onUnloadScript(std::string_view fileName);
	private:
		void attemptResolveInProgress();
		virtual void setApiSetupFuncs(v8pp::context* ctx) override;
		void exposeStandardFuncs(v8pp::context* ctx);
		void exposeClientPlayer(v8pp::context* ctx);
		void exposeKeyboardHandler(v8pp::context* ctx);
		void exposeLocalAttrs(v8pp::context* ctx);
		void exposeNetworkedPlayerClass(v8pp::context* ctx);
		void exposeNetworkPlayerManagerFunctions(v8pp::context* ctx);
		void exposeRmlUIFunctions(v8pp::context* ctx);
		std::map<std::string, std::unique_ptr<Networking::NetworkAsset<Script>>> _scripts{};
		std::map<std::string, std::unique_ptr<Networking::NetworkAsset<Script>>> _scriptsInProgress{};
		Entities::ClientPlayer* _clientPlayer{ nullptr };

		std::mutex _scriptsToLoadMutex{};
		std::set<std::string> _scriptsToLoad{};

		std::mutex _scriptsToUnloadMutex{};
		std::set<std::string> _scriptsToUnload{};
	};
}