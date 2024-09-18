#include <RmlUi/Debugger.h>
#include "ViciClient/include/Scenes/Scene.hpp"
#include "ViciClient/include/GUI/ViciGuiHost.hpp"
#include "ViciClient/include/GUI/RmlUi_Platform_SDL.h"

namespace Scenes {
	Scene::Scene(std::string_view name) : _rmlContext { GUI::ViciGuiHost::getContext(name) }, _contextName{ name } {}

	Scene::~Scene(){
		GUI::ViciGuiHost::destroyContext(_contextName);
	}

	void Scene::update() {
		_rmlContext.Update();
	}

	void Scene::render([[maybe_unused]] SDL_Renderer* renderer) {
		_rmlContext.Render();
	}

	void Scene::handleEvents(SDL_Event& event) {
		Rml::RmlSDL::InputEventHandler(&_rmlContext, event);
	}

	Rml::Context& Scene::getContext() {
		return _rmlContext;
	}
}