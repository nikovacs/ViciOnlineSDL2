#pragma once
#include <string_view>
#include <SDL2/SDL.h>
#include <RmlUi/Core.h>

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
	};
}