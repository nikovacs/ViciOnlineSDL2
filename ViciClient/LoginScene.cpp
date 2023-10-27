#include "LoginScene.hpp"

#include <iostream>

namespace Scenes {
    LoginButtonListener::LoginButtonListener(std::function<void(std::string_view)>& onLoginCallback) : _callbackOnProcess{ onLoginCallback } {}

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
		Rml::ElementDocument* document = getContext().LoadDocumentFromMemory(R""(
			<rml>
                <head>
                    <title>User Login</title>
					<style>
						body {
                            font-family: "Applestorm";
                        }

                        .login-box {
                            width: 300px;
                            padding: 20px;
                            border: 1px solid #ccc;
                            border-radius: 5px;
                            text-align: center;
                            display: flex;
                            justify-content: center;
                            align-items: center;
                            margin: 0;
                        }

                        input[type="text"],
                        input[type="password"],
                        button {
                            width: 100%;
                            padding: 10px;
                            margin-bottom: 10px;
                            border: 1px solid #ccc;
                            border-radius: 3px;
                        }
					</style>
                </head>
                <body>
                    <div class="login-box">
                        <h2>User Login</h2>
                        <form>
                            <input id="username-field" type="text" placeholder="Username" required></input><br></br>
                            <input id="password-field" type="password" placeholder="Password" required></input><br></br>
                            <button id="loginButton" type="submit">Login</button>
                        </form>
                    </div>
                </body>
            </rml>
		)"");
        document->GetElementById("loginButton")->AddEventListener("click", &_loginButtonListener);
        document->Show();
	}

	void LoginScene::update() {
		Scene::update();
	}

	void LoginScene::render(SDL_Renderer* renderer) {
		Scene::render(renderer);
	}
}