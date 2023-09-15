#pragma once
#include "Scene.hpp"
#include <string>

namespace Scenes {
	class LoginScene : public Scene {
	public:
		LoginScene(std::string_view sceneName);
		void update() override;
		void render(SDL_Renderer* renderer) override;
	};
}