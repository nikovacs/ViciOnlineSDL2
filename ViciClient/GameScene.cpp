#include "GameScene.hpp"
#include "ViciClient.hpp"
#include "NetworkAsset.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include "Animation.hpp"
#include "SingleLevel.hpp"
#include "Camera.hpp"
#include "ClientPlayerManager.hpp"

#include "KeyboardInputHandler.hpp" // TEMPORARY

namespace Scenes {
	GameScene* GameScene::instance = nullptr;
	
	GameScene::GameScene(std::string_view sceneName, int x, int y, int w, int h, int dir, std::string_view animation, std::string_view world)
		: Scene(sceneName), _udpClient{ "localhost", 8424 }, _scriptLoader{} {
		_camera.initialize();
		_clientPlayer = std::make_unique<Entities::ClientPlayer>(animation, x, y, dir);
		_camera.setFocusObject(_clientPlayer.get());
		_clientPlayer->setWidth(w);
		_clientPlayer->setHeight(h);
		_world = std::make_unique<Networking::NetworkAsset<Levels::Level>>(world);
		instance = this;
	}

	GameScene::~GameScene() {}

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

		Networking::ClientPlayerManager::update();
		_scriptLoader.update();
		
		if (_world->getValue()) {
			_world->getValue()->update();
			_levelWatchingManager.update(*_world->getValue());
		}
		
		if (_world->getValue()) {
			_camera.update(*_world->getValue());
		}

		Scene::update(); // base class update
	}

	void GameScene::render(SDL_Renderer* renderer) {
		if (_world->getValue())
			_world->getValue()->render(renderer);
		if (_clientPlayer)
			_clientPlayer->render(renderer);
		Networking::ClientPlayerManager::render(renderer);

		Scene::render(renderer); // base class render
	}

	Client::Camera& GameScene::getCamera() {
		return _camera;
	}

	void GameScene::start() {
		_networkThread = std::make_unique<std::thread>(&Networking::UdpClient::start, &_udpClient);
	}

	void GameScene::stop() {
		_udpClient.stop();
		_networkThread->join();
	}
}