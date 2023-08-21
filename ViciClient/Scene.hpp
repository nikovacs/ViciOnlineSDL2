#pragma once
#include <map>
#include <string>
#include <memory>
#include <SDL2/SDL.h>
#include <mutex>
#include "nlohmann/json.hpp"
#include "enet/enet.h"

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
		void newGameScene(nlohmann::json& json);
		static SceneManager* instance;
	private:
		Scene* _currentScene{ nullptr };
		std::map<std::string_view, std::unique_ptr<Scene>> _scenes{};
		std::mutex _sceneMutex{};
	};
}