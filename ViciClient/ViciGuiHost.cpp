#include "ViciGuiHost.hpp"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/RenderInterface.h>

namespace GUI {
	ViciGuiHost::ViciGuiHost(SDL_Renderer* renderer)
		: _renderer{ renderer }, _renderInterface { renderer }, _systemInterface{} {
		Rml::SetRenderInterface(&_renderInterface);
		Rml::SetSystemInterface(&_systemInterface);
		Rml::Initialise();
	}

	ViciGuiHost::~ViciGuiHost() {
		Rml::Shutdown();
	}
}