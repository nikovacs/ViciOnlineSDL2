#pragma once

#include <SDL2/SDL.h>
#include <string_view>
#include <map>
#include <mutex>
#include <memory>
#include "ViciClient/include/Scenes/Scene.hpp"

namespace Scenes {
	class SceneManager {
	public:
		SceneManager();
		~SceneManager();
		void update();
		void render(SDL_Renderer* renderer);
		void handleEvents(SDL_Event* event);
		void setScene(std::string_view name);
		Scene& getScene(std::string_view name);
		void newGameScene();
		void newLoginScene(std::function<void()> onLoginCallback);
		static SceneManager* instance;
	private:
		Scene* _currentScene{ nullptr };
		std::map<std::string_view, std::unique_ptr<Scene>> _scenes{};
		std::recursive_mutex _sceneMutex{};
	};
}