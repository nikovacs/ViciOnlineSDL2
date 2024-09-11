#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Core.h>
#include <RmlUi/Debugger.h>
#include <SDL2/SDL.h>
#include <iostream>
#include "ViciClient/include/GUI/ViciGuiHost.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/RmlElementDeletionListenerPlugin.hpp"

namespace GUI {
	std::map<std::string, Rml::Context*> ViciGuiHost::_contexts{};
	SDL_Window* ViciGuiHost::_window = nullptr;
	SDL_Renderer* ViciGuiHost::_renderer = nullptr;
	Rml::SystemInterface_SDL* ViciGuiHost::_systemInterface = nullptr;
	Rml::RenderInterface_SDL* ViciGuiHost::_renderInterface = nullptr;
	JS::RmlElementDeletionListenerPlugin* ViciGuiHost::_elementDeletionListenerPlugin = nullptr;
	Rml::FileInterface* ViciGuiHost::_fileInterface = nullptr;


	void ViciGuiHost::initialize(SDL_Window* window, SDL_Renderer* renderer) {
		_window = window;
		_renderer = renderer;
		_fileInterface = new Rml::RmlUi_FileInterface();
		_systemInterface = new Rml::SystemInterface_SDL();
		_renderInterface = new Rml::RenderInterface_SDL(renderer);
		Rml::SetFileInterface(_fileInterface);
		Rml::SetRenderInterface(_renderInterface);
		Rml::SetSystemInterface(_systemInterface);
		Rml::Initialise();
		std::cout << "rml initialized\n";
		std::cout << "font loaded: " << Rml::LoadFontFace("ApplestormChalkboard.otf", true) << '\n';

		_elementDeletionListenerPlugin = new JS::RmlElementDeletionListenerPlugin();
		Rml::RegisterPlugin(_elementDeletionListenerPlugin);
	}

	void ViciGuiHost::deinitialize() {
		std::cout << "rml deinitialized\n";
		Rml::Shutdown();
		_contexts.clear();
		delete _elementDeletionListenerPlugin;
		free(_systemInterface);
		free(_renderInterface);
		free(_fileInterface);
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
		std::cout << "destroying context: " << name << '\n';
		if (!_contexts.contains(name.data())) return;
		Rml::RemoveContext(name.data());
		_contexts.erase(name.data());
	}
}