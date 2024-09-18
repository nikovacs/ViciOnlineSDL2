#pragma once

#include <string_view>
#include "header_wrappers/sdl_wrapper.h"
#include "header_wrappers/rmlui_wrapper.h"
#include <string>

namespace Scenes {
	class Scene {
	public:
		Scene(std::string_view name);
		virtual ~Scene();
		virtual void update();
		virtual void render(SDL_Renderer* renderer);
		void handleEvents(SDL_Event& event);
	protected:
		Rml::Context& getContext();
	private:
		Rml::Context& _rmlContext;
		std::string _contextName{};
	};
}