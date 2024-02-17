#include "LoginScene.hpp"
#include "RmlUi/Debugger.h"
#include <iostream>

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
        Rml::Context* rmlContext = &getContext();
        Rml::Debugger::Initialise(rmlContext);
        Rml::Debugger::SetContext(rmlContext);
        Rml::Debugger::SetVisible(true);
        
        Rml::ElementDocument* document = getContext().LoadDocument("loginScene.html");
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