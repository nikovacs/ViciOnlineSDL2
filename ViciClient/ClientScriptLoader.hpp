#pragma once

#include "../ViciEngine/ScriptLoader.hpp"
#include <map>
#include <set>
#include <string>
#include <memory>
#include "NetworkAsset.hpp"

using namespace std::literals;

namespace JS {
	class ClientScriptLoader : public ScriptLoader {
	public:
		ClientScriptLoader();
		virtual ~ClientScriptLoader() = default;
		virtual void loadScript(std::string_view fileName) override;
		virtual void unloadScript(std::string_view fileName) override;
		virtual void trigger(std::string_view functionName, std::string_view fileName = ""sv) override;
		void update();
	private:
		void attemptResolveInProgress();
		std::map<std::string, std::unique_ptr<Networking::NetworkAsset<Script>>> _scripts;
		std::map<std::string, std::unique_ptr<Networking::NetworkAsset<Script>>> _scriptsInProgress;
	};
}