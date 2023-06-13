#pragma once
#include <map>
#include <string>
#include <memory>
#include <SDL2/SDL.h>

namespace Scenes {
	class Scene {
	public:
		Scene() {};
		virtual ~Scene() {};
		virtual void update() = 0;
		virtual void render(SDL_Renderer* renderer) = 0;
	};

	class SceneManager {
	public:
		SceneManager();
		~SceneManager();
		void update();
		void render(SDL_Renderer* renderer);
		void setScene(std::string_view name);

	private:
		Scene* _currentScene;
		std::map<std::string_view, std::unique_ptr<Scene>> _scenes;
	};
}