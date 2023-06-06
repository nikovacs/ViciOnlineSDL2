#pragma once
#include <map>
#include <string>
#include <memory>

namespace Scenes {
	class Scene {
	public:
		Scene() {};
		virtual ~Scene() {};
		virtual void update() = 0;
		virtual void render() = 0;
	};

	class SceneManager {
	public:
		SceneManager();
		virtual ~SceneManager();
		void update();
		void render();
		void setScene(std::string_view name);

	private:
		Scene* _currentScene;
		std::map<std::string_view, std::unique_ptr<Scene>> _scenes;
	};
}