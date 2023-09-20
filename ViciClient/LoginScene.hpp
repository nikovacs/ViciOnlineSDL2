#pragma once
#include "Scene.hpp"
#include <string>
#include <functional>

namespace Scenes {
	class LoginButtonListener : public Rml::EventListener {
	public:
		LoginButtonListener(std::function<void()>& callbackOnProcess);
		void ProcessEvent(Rml::Event& event);
	private:
		std::function<void()>& _callbackOnProcess;
	};

	class LoginScene : public Scene {
	public:
		LoginScene(std::string_view sceneName, std::function<void()> onLoginCallback);
		void update() override;
		void render(SDL_Renderer* renderer) override;
	private:
		LoginButtonListener _loginButtonListener;
		std::function<void()> _onLoginCallback{};
	};
}