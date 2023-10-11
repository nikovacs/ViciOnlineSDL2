#pragma once

#include "../ViciEngine/ScriptLoader.hpp"
#include <map>
#include <set>
#include <string>
#include <memory>
#include "NetworkAsset.hpp"
#include "ClientPlayer.hpp"

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
		void setApiSetupFuncs();
		void exposeClientPlayer();
		void exposeKeyboardHandler();
		void exposeLocalAttrs();
		std::map<std::string, std::unique_ptr<Networking::NetworkAsset<Script>>> _scripts{};
		std::map<std::string, std::unique_ptr<Networking::NetworkAsset<Script>>> _scriptsInProgress{};
		Entities::ClientPlayer* _clientPlayer{ nullptr };
	};
}