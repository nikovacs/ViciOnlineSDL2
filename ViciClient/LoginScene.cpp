#include "LoginScene.hpp"

namespace Scenes {
	LoginScene::LoginScene(std::string_view sceneName) : Scene(sceneName) {
		Rml::ElementDocument* document = getContext().LoadDocumentFromMemory(R""(
			<rml>
                <head>
                <title>Example</title>
                <style>
                    body
                    {
                        position: absolute;
                        top: 50px;
                        left: 50px;
                        width: 500px;
                        height: 500px;
                        background-color: #ccc;
                    }
                    div
                    {
                        display: block;
                        height: 150px;
                        width: 200px; 
                        background-color: #f00;
                    }
                </style>
                </head>
                <body>
                    <div/>
                </body>
            </rml>
		)"");
	}

	void LoginScene::update() {
		Scene::update();
	}

	void LoginScene::render(SDL_Renderer* renderer) {
		Scene::render(renderer);
	}
}