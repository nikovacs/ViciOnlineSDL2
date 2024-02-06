#pragma once
#include <SDL2/SDL.h>
#include <map>
#include <RmlUi/Core/Context.h>
#include <string>
#include "RmlUi_Renderer_SDL.h"
#include "RmlUi_Platform_SDL.h"
#include "RmlUi_FileInterface.h"
#include "RmlElementDeletionListenerPlugin.hpp"

namespace GUI {
	class ViciGuiHost {
	public:
		ViciGuiHost() = delete;
		~ViciGuiHost() = delete;
		static void initialize(SDL_Window* window, SDL_Renderer* renderer);
		static void deinitialize();
		static Rml::Context& getContext(std::string_view name);
		static void destroyContext(std::string_view name);
	private:
		static std::map<std::string, Rml::Context*> _contexts;
		static SDL_Window* _window;
		static SDL_Renderer* _renderer;
		static Rml::FileInterface* _fileInterface;
		static Rml::SystemInterface_SDL* _systemInterface;
		static Rml::RenderInterface_SDL* _renderInterface;
		static JS::RmlElementDeletionListenerPlugin* _elementDeletionListenerPlugin;
	};
}