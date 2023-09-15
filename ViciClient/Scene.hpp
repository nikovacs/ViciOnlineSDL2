#pragma once
#include <map>
#include <string>
#include <memory>
#include <SDL2/SDL.h>
#include <mutex>
#include "nlohmann/json.hpp"
#include "enet/enet.h"
#include <RmlUi/Core.h>
#include <SDL2/SDL.h>

namespace Scenes {
	class Scene {
	public:
		Scene(std::string_view name);
		virtual ~Scene();
		virtual void update();
		virtual void render(SDL_Renderer* renderer);
		void handleEvents(SDL_Event& event);
		virtual void start();
		virtual void stop();
	protected:
		Rml::Context& getContext();
	private:
		Rml::Context& _rmlContext;
	};

	class SceneManager {
	public:
		SceneManager();
		~SceneManager();
		void update();
		void render(SDL_Renderer* renderer);
		void handleEvents(SDL_Event* event);
		void setScene(std::string_view name);
		void newGameScene(nlohmann::json& json);
		void newLoginScene();
		void start();
		void stop();
		static SceneManager* instance;
	private:
		Scene* _currentScene{ nullptr };
		std::map<std::string_view, std::unique_ptr<Scene>> _scenes{};
		std::mutex _sceneMutex{};
	};
}