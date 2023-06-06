#pragma once
#include "Scene.h"

namespace Scenes {
	class GameScene : public Scene {
	public:
		GameScene();
		~GameScene() override;
		void update() override;
		void render() override;
	};
}