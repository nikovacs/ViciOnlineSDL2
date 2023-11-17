#pragma once

#include "../ViciEngine/ScriptLoader.hpp"
#include <map>
#include <set>
#include <string>
#include <memory>
#include "NetworkAsset.hpp"
#include "ClientPlayer.hpp"
#include <v8pp/context.hpp>

using namespace std::literals;

namespace JS {
	class ClientScriptLoader : public ScriptLoader {
	public:
		ClientScriptLoader();
		virtual ~ClientScriptLoader() override;
		virtual void loadScript(std::string_view fileName) override;
		virtual void unloadScript(std::string_view fileName) override;
		virtual void trigger(std::string_view functionName, std::string_view fileName = ""sv) override;
		void update();
		void setClientPlayer(Entities::ClientPlayer* pl);
	private:
		void attemptResolveInProgress();
		void setApiSetupFuncs(v8pp::context* ctx);
		void exposeClientPlayer(v8pp::context* ctx);
		void exposeKeyboardHandler(v8pp::context* ctx);
		void exposeLocalAttrs(v8pp::context* ctx);
		void exposeNetworkedPlayerClass(v8pp::context* ctx);
		void exposeNetworkPlayerManagerFunctions(v8pp::context* ctx);
		std::map<std::string, std::unique_ptr<Networking::NetworkAsset<Script>>> _scripts{};
		std::map<std::string, std::unique_ptr<Networking::NetworkAsset<Script>>> _scriptsInProgress{};
		Entities::ClientPlayer* _clientPlayer{ nullptr };
	};
}