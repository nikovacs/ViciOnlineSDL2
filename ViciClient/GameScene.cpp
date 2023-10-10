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
		_clientPlayer = std::make_unique<Entities::ClientPlayer>(json);
		_scriptLoader.setClientPlayer(_clientPlayer.get());
		_camera.setFocusObject(_clientPlayer.get());
		_clientPlayer->setWidth(json["w"]);
		_clientPlayer->setHeight(json["h"]);
		_world = std::make_unique<Networking::NetworkAsset<Levels::Level>>(json["world"]);
		_camera.setScale(json["cameraZoom"]);
		_scriptLoader.loadScript("testscript.js");
	}

	Client::Camera& GameScene::getCamera() {
		return _camera;
	}
}