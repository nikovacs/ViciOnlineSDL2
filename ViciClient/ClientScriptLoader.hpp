#include "../ViciEngine/ScriptLoader.hpp"

namespace JS {
	class ClientScriptLoader : public ScriptLoader {
	public:
		ClientScriptLoader() = default;
		virtual ~ClientScriptLoader() = default;
		virtual void loadScript(std::string_view fileName) override;
		virtual void unloadScript(std::string_view fileName) override;
	};
}