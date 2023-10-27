#pragma once
#include "Scene.hpp"
#include <string>
#include <functional>

namespace Scenes {
	class LoginButtonListener : public Rml::EventListener {
	public:
		LoginButtonListener(std::function<void(std::string_view)>& callbackOnProcess);
		void ProcessEvent(Rml::Event& event);
	private:
		std::function<void(std::string_view)>& _callbackOnProcess;
	};

	class LoginScene : public Scene {
	public:
		LoginScene(std::string_view sceneName, std::function<void(std::string_view)> onLoginCallback);
		void update() override;
		void render(SDL_Renderer* renderer) override;
	private:
		LoginButtonListener _loginButtonListener;
		std::function<void(std::string_view)> _onLoginCallback{};
	};
}