#include "ViciGuiHost.hpp"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Core.h>
#include <RmlUi/Debugger.h>
#include <SDL2/SDL.h>

#include <iostream>

namespace GUI {
	std::map<std::string, Rml::Context*> ViciGuiHost::_contexts{};
	SDL_Window* ViciGuiHost::_window = nullptr;
	SDL_Renderer* ViciGuiHost::_renderer = nullptr;
	Rml::SystemInterface_SDL* ViciGuiHost::_systemInterface = nullptr;
	Rml::RenderInterface_SDL* ViciGuiHost::_renderInterface = nullptr;


	void ViciGuiHost::initialize(SDL_Window* window, SDL_Renderer* renderer) {
		_window = window;
		_renderer = renderer;
		_systemInterface = new Rml::SystemInterface_SDL();
		_renderInterface = new Rml::RenderInterface_SDL(renderer);
		Rml::SetRenderInterface(_renderInterface);
		Rml::SetSystemInterface(_systemInterface);
		Rml::Initialise();
		std::cout << "rml initialized\n";
		std::cout << "font loaded: " << Rml::LoadFontFace("ApplestormChalkboard.otf", true) << '\n';
	}

	void ViciGuiHost::deinitialize() {
		std::cout << "rml deinitialized\n";
		Rml::Shutdown();
		_contexts.clear();
		
		free(_systemInterface);
		free(_renderInterface);
	}
	
	Rml::Context& ViciGuiHost::getContext(std::string_view name) {
		if (!_contexts.contains(name.data())) {
			int screenWidth, screenHeight;
			SDL_GetWindowSize(_window, &screenWidth, &screenHeight);
			Rml::Vector2i screenSize{ screenWidth, screenHeight };
			Rml::Context* context{ Rml::CreateContext(name.data(), screenSize) };
			_contexts.insert({ name.data(), context });
		}
		return *_contexts[name.data()];
	}
	
	
	void ViciGuiHost::destroyContext(std::string_view name) {
		Rml::RemoveContext(name.data());
		_contexts.erase(name.data());
	}
}