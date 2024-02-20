#pragma once
#include "Scene.hpp"
#include <string>
#include <functional>

namespace Scenes {
	class LoginScene;

	class LoginButtonListener : public Rml::EventListener {
	public:
		LoginButtonListener(std::function<void(std::string_view)>& callbackOnProcess);
		void ProcessEvent(Rml::Event& event);
	private:
		std::function<void(std::string_view)>& _callbackOnProcess;
	};

	class ContinueAsGuestButtonListener : public Rml::EventListener {
	public:
		ContinueAsGuestButtonListener(LoginScene* parent);
		void ProcessEvent(Rml::Event& event);
	private:
		LoginScene* _parent;
	};

	class LoginScene : public Scene {
	public:
		LoginScene(std::string_view sceneName, std::function<void(std::string_view)> onLoginCallback);
		void update() override;
		void render(SDL_Renderer* renderer) override;
		void loginAsGuest();
	private:
		bool getCustomDeviceId(std::string& out);
		ContinueAsGuestButtonListener _continueAsGuestButtonListener;
		LoginButtonListener _loginButtonListener;
		std::function<void(std::string_view)> _onLoginCallback{};
	};
}