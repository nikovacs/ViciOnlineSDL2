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

	GameScene::GameScene(std::string_view sceneName, std::string_view serverUrl, int serverPort) : Scene{ sceneName }, _udpClient{ serverUrl, serverPort }, _scriptLoader{} {
		_networkThread = std::make_unique<std::thread>(&Networking::UdpClient::start, &_udpClient);
		instance = this;
	}

	GameScene::~GameScene() {
		_udpClient.stop();
		_networkThread->join();
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

	void GameScene::loadInitPlayerData(nlohmann::json& json) {
		int x = json["x"];
		int y = json["y"];
		int w = json["w"];
		int h = json["h"];
		int dir = json["dir"];
		std::string_view animation = json["animation"];
		std::string_view world = json["world"];
		float cameraZoom = json["cameraZoom"];

		_clientPlayer = std::make_unique<Entities::ClientPlayer>(animation, x, y, dir);
		_camera.setFocusObject(_clientPlayer.get());
		_clientPlayer->setWidth(w);
		_clientPlayer->setHeight(h);
		_world = std::make_unique<Networking::NetworkAsset<Levels::Level>>(world);
		_camera.setScale(cameraZoom);
	}

	Client::Camera& GameScene::getCamera() {
		return _camera;
	}
}