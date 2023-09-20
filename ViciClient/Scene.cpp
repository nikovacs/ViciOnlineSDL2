#include "Scene.hpp"
#include "ViciGuiHost.hpp"
#include <RmlUi/Debugger.h>
#include "RmlUi_Platform_SDL.h"

namespace Scenes {
	Scene::Scene(std::string_view name) : _rmlContext { GUI::ViciGuiHost::getContext(name) } {
		Rml::Debugger::Initialise(&_rmlContext);
	}

	Scene::~Scene(){
		GUI::ViciGuiHost::destroyContext(_rmlContext.GetName());
	}

	void Scene::update() {
		_rmlContext.Update();
	}

	void Scene::render(SDL_Renderer* renderer) {
		_rmlContext.Render();
	}

	void Scene::handleEvents(SDL_Event& event) {
		Rml::RmlSDL::InputEventHandler(&_rmlContext, event);
	}

	Rml::Context& Scene::getContext() {
		return _rmlContext;
	}
}