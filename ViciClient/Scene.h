#pragma once
#include <map>
#include <string>

namespace Scenes {
	class Scene {
	public:
		Scene();
		virtual ~Scene();
		
		virtual void initialize() = 0;
		virtual void update() = 0;
		virtual void render() = 0;
		virtual void uninitialize() = 0;
		
	};

	static class SceneManager {
	public:
		static void initialize();
		static void update();
		static void render();
		static void uninitialize();

		static void setScene(std::string_view name);
		static void resetScene(std::string_view name);

	private:
		static std::reference_wrapper<Scene> _currentScene;
		static std::map<std::string_view, Scene> _scenes;
	};
}