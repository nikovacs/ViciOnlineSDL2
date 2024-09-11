#pragma once

#include <string>
#include <functional>
#include "ViciClient/include/Scenes/Scene.hpp"

namespace Scenes {
	class LoginScene;

	class LoginButtonListener : public Rml::EventListener {
	public:
		LoginButtonListener(LoginScene* parent);
		void ProcessEvent(Rml::Event& event);
	private:
		LoginScene* _parent;
	};

	class ContinueAsGuestButtonListener : public Rml::EventListener {
	public:
		ContinueAsGuestButtonListener(LoginScene* parent);
		void ProcessEvent(Rml::Event& event);
	private:
		LoginScene* _parent;
	};

	class RegisterNowButtonListener : public Rml::EventListener {
	public:
		RegisterNowButtonListener(LoginScene* parent);
		void ProcessEvent(Rml::Event& event);
	private:
		LoginScene* _parent;
	};

	class BackButtonListener : public Rml::EventListener {
	public:
		BackButtonListener(LoginScene* parent);
		void ProcessEvent(Rml::Event& event);
	private:
		LoginScene* _parent;
	};

	class RegisterButtonListener : public Rml::EventListener {
	public:
		RegisterButtonListener(LoginScene* parent);
		void ProcessEvent(Rml::Event& event);
	private:
		LoginScene* _parent;
	};

	class LoginScene : public Scene {
	public:
		LoginScene(std::string_view sceneName, std::function<void()> onLoginCallback);
		virtual ~LoginScene();
		void update() override;
		void render(SDL_Renderer* renderer) override;
	private:
		Rml::ElementDocument* _loginSceneDoc{ nullptr };
		Rml::ElementDocument* _registerSceneDoc{ nullptr };

		static bool _getCustomDeviceId(std::string& out);
		void _loginAsGuest(bool goToGameScene = true);
		void _addEmailUsernamePassword(std::string_view email, std::string_view username, std::string_view password);
		void _unlinkCustomId();
		static bool _isValidEmail(std::string_view email);
		
		LoginButtonListener _loginButtonListener;
		std::function<void()> _onLoginCallback{};
		friend class LoginButtonListener;

		ContinueAsGuestButtonListener _continueAsGuestButtonListener;
		friend class ContinueAsGuestButtonListener;

		RegisterNowButtonListener _registerNowButtonListener;
		friend class RegisterNowButtonListener;

		BackButtonListener _backButtonListener;
		friend class BackButtonListener;

		RegisterButtonListener _registerButtonListener;
		friend class RegisterButtonListener;
	};
}