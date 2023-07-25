#include "GameScene.hpp"
#include "ViciClient.hpp"
#include "NetworkAsset.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include "Animation.hpp"
#include "SingleLevel.hpp"
#include "Camera.hpp"

#include "KeyboardInputHandler.hpp" // TEMPORARY

namespace Scenes {
	GameScene* GameScene::instance = nullptr;
	
	GameScene::GameScene() {
		instance = this;
	}

	GameScene::~GameScene() {

	}

	void GameScene::initialize() {
		_camera.initialize();
		_clientPlayer = std::make_unique<Entities::ClientPlayer>("player_idle.vani", 0, 0, 3);
		_camera.setFocusObject(_clientPlayer.get());
		_clientPlayer->setWidth(13);
		_clientPlayer->setHeight(23);
		_level = std::make_unique<Networking::NetworkAsset<Levels::Level>>("newlevel.vlvl");
	}

	void GameScene::update() {
		if (_clientPlayer) {
			_clientPlayer->update();
			// the below four if statements are temporary
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
		}
		
		if (_level->getValue()) {
			_camera.update(*_level->getValue());
		}
	}

	void GameScene::render(SDL_Renderer* renderer) {
		if (_level->getValue())
			_level->getValue()->render(renderer);
		if (_clientPlayer)
			_clientPlayer->render(renderer);
	}

	Client::Camera& GameScene::getCamera() {
		return _camera;
	}
}