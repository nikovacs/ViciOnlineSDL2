#include "LoginScene.hpp"

#include <iostream> 

namespace Scenes {
    LoginButtonListener::LoginButtonListener(std::function<void()>& onLoginCallback) : _callbackOnProcess{ onLoginCallback } {}

    void LoginButtonListener::ProcessEvent(Rml::Event& event) {
        _callbackOnProcess();
    }

    LoginScene::LoginScene(std::string_view sceneName, std::function<void()> onLoginCallback) : Scene{ sceneName }, _onLoginCallback{ onLoginCallback }, _loginButtonListener{ _onLoginCallback } {
		Rml::ElementDocument* document = getContext().LoadDocumentFromMemory(R""(
			<rml>
                <head>
                    <title>User Login</title>
					<style>
						body {
                            font-family: "Applestorm";
                            /* TODO: I don't have the knowledge of how a scene works to solve this right now, but only hald the page is considered body?*/
                            /* My assumption is only half of the screen is rendered through rmlui, the other half is something else*/
                            background: gray;
                        }

                        .title-box,
                        .login-box {
                            display: flex;
                            width: 300px;
                            padding: 20px;
                            border-radius: 5px;
                            text-align: center;
                            justify-content: center;
                            align-items: center;
                            margin: auto;
                        }

                        .title-box {
                            font-effect: glow( 2px #ee9 );
                            font-size: 48px;
                        }
                        
                        input[type="text"],
                        input[type="password"],
                        button {
                            width: 100%;
                            padding: 10px;
                            margin-bottom: 10px;
                            border-radius: 3px;
                            border: 1px solid #CCC000; /* warning presents with this line, but removal deletes the outlines*/
                            font-size: 24px;
                        }

                        label {
                            font-size: 24px;
                        }
					</style>
                </head>
                <body>
                    <div class="title-box">
                        <h2>User Login</h2>
                    </div>

                    <div class="login-box">
                        <form>
                            <div class="input-container">
                                <label for="username">Username</label>
                                <input type="text" id="username" required></input>
                            </div>
                            <div class="input-container">
                                <label for="password">Password</label>
                                <input type="password" required></input>
                            </div>
                            <br></br>
                            <br></br>
                            <button id="loginButton" type="submit" onclick="">Login</button>
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