#include "LoginScene.hpp"

namespace Scenes {
    LoginButtonListener::LoginButtonListener(std::function<void(std::string_view)>& onLoginCallback) : _callbackOnProcess{ onLoginCallback } {
    }

    void LoginButtonListener::ProcessEvent(Rml::Event& event) {
        auto userField = event.GetCurrentElement()->GetOwnerDocument()->GetElementById("username-field");
        Rml::String username = "";
        if (userField) {
            Rml::Variant* variant = userField->GetAttribute("value");
            if (variant) {
				username = variant->Get<Rml::String>();
			}
        }
        if (!username.empty()) {
            _callbackOnProcess(username);
        }
    }

    LoginScene::LoginScene(std::string_view sceneName, std::function<void(std::string_view)> onLoginCallback) : Scene{ sceneName }, _onLoginCallback{ onLoginCallback }, _loginButtonListener{ _onLoginCallback } {
        Rml::ElementDocument* document = getContext().LoadDocument("__vici_internal__loginScene.html");
        //document->GetElementById("loginButton")->AddEventListener("click", &_loginButtonListener);
        document->Show();
	}

	void LoginScene::update() {
		Scene::update();
	}

	void LoginScene::render(SDL_Renderer* renderer) {
        Scene::render(renderer);
	}
}