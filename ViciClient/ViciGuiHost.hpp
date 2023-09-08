#pragma once

#include <map>
#include <RmlUi/Core/Context.h>
#include <string>
#include "RmlUi_Renderer_SDL.h"
#include "RmlUi_Platform_SDL.h"
#include <SDL2/SDL.h>

namespace GUI {
	class ViciGuiHost {
	public:
		ViciGuiHost(SDL_Renderer* renderer);
		~ViciGuiHost();
	private:
		Rml::SystemInterface_SDL _systemInterface;
		Rml::RenderInterface_SDL _renderInterface;
		SDL_Renderer* _renderer;
	};
}