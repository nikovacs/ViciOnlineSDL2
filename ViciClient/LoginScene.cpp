#include "LoginScene.hpp"

namespace Scenes {
	LoginScene::LoginScene(std::string_view sceneName) : Scene(sceneName) {
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
                            <input type="text" placeholder="Username" required></input><br></br>
                            <input type="password" placeholder="Password" required></input><br></br>
                            <button type="submit">Login</button>
                        </form>
                    </div>
                </body>
            </rml>
		)"");
        document->Show();
	}

	void LoginScene::update() {
		Scene::update();
	}

	void LoginScene::render(SDL_Renderer* renderer) {
		Scene::render(renderer);
	}
}