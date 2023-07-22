#include "GameScene.hpp"
#include "ViciClient.hpp"
#include "NetworkAsset.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include "Animation.hpp"
#include "SingleLevel.hpp"

#include "KeyboardInputHandler.hpp" // TEMPORARY

namespace Scenes {
	Scenes::GameScene::GameScene() {
	}

	Scenes::GameScene::~GameScene() {

	}

	void Scenes::GameScene::initialize() {
		_clientPlayer = std::make_unique<Entities::ClientPlayer>("player_idle.vani", 0, 0, 3);
		_level = std::make_unique<Networking::NetworkAsset<Levels::Level>>("newlevel.vlvl");
	}

	void Scenes::GameScene::update() {
		if (_clientPlayer) {
			_clientPlayer->update();
			//_clientPlayer->setPosition(_clientPlayer->getX() + 1, _clientPlayer->getY() + 1); // TEMPORARY
			if (Handlers::KeyboardInputHandler::isKeyDown("Down"))
				_clientPlayer->setPosition(_clientPlayer->getX(), _clientPlayer->getY() + 3);
			if (Handlers::KeyboardInputHandler::isKeyDown("Up"))
				_clientPlayer->setPosition(_clientPlayer->getX(), _clientPlayer->getY() - 3);
			if (Handlers::KeyboardInputHandler::isKeyDown("Left"))
				_clientPlayer->setPosition(_clientPlayer->getX() - 3, _clientPlayer->getY());
			if (Handlers::KeyboardInputHandler::isKeyDown("Right"))
				_clientPlayer->setPosition(_clientPlayer->getX() + 3, _clientPlayer->getY());
		}
		if (_level->getValue()) {
			_level->getValue()->update();
			_camera.update(*(_level->getValue()));
		}
		
	}

	void Scenes::GameScene::render(SDL_Renderer* renderer) {
		if (_level->getValue())
			_level->getValue()->render(renderer);
		if (_clientPlayer)
			_clientPlayer->render(renderer);
	}
}