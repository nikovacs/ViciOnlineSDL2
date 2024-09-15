#include "header_wrappers/sdl_wrapper.h"
#include "ViciClient/include/Scenes/GameScene.hpp"
#include "ViciClient/include/Camera.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/ViciAsyncDocumentLoader.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/ViciJSEventListenerHandler.hpp"
#include "ViciClient/include/Levels/SingleLevel.hpp"
#include "ViciClient/include/Networking/ClientPlayerManager.hpp"
#include "ViciClient/include/Networking/NetworkAsset.hpp"
#include "ViciClient/include/ViciClient.hpp"
#include <RmlUi/Debugger.h>
#include <memory>

namespace Scenes {
GameScene *GameScene::instance = nullptr;

GameScene::GameScene(std::string_view sceneName, std::string_view serverUrl, int serverPort)
    : Scene{sceneName}, _udpClient{serverUrl, serverPort}, _scriptLoader{},
      _animator{gottanis::Animator::getInstance()}, _gottanisRenderer{ViciClient::instance->getRenderer()} {
    _networkThread = std::make_unique<std::thread>(&Networking::UdpClient::start, &_udpClient);
    instance = this;

    _animator.setFileLoader(&_gottanisFileLoader);
	_animator.setRenderer(&_gottanisRenderer);
    // set renderer, collider manager, etc

    Rml::Context *rmlContext = &getContext();
    Rml::Debugger::Initialise(rmlContext);
    Rml::Debugger::SetContext(rmlContext);
    Rml::Debugger::SetVisible(true);
}

GameScene::~GameScene() {
    _udpClient.stop();
    _networkThread->join();
    GUI::ViciGuiHost::destroyContext("GameScene");
    JS::ViciJSEventListenerHandler::deinitialize();
}

void GameScene::update() {
    if (_clientPlayer) {
        _clientPlayer->update();
    }

    Networking::ClientPlayerManager::update();
    JS::ViciAsyncDocumentLoader::attemptResolve();
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

void GameScene::render(SDL_Renderer *renderer) {
    if (_world->getValue())
        _world->getValue()->render(renderer);
    if (_clientPlayer)
        _clientPlayer->render(renderer);
    Networking::ClientPlayerManager::render(renderer);

    Scene::render(renderer); // base class render
}

void GameScene::loadInitPlayerData(nlohmann::json &json) {
    _clientPlayer = std::make_unique<Entities::ClientPlayer>(json);
    _scriptLoader.setClientPlayer(_clientPlayer.get());
    _camera.setFocusObject(_clientPlayer.get());
    _clientPlayer->setWidth(json["w"]);
    _clientPlayer->setHeight(json["h"]);
    _world = std::make_unique<Networking::NetworkAsset<Levels::Level>>(json["world"].get<std::string>());
    _camera.setScale(json["cameraZoom"], false);
}

Client::Camera &GameScene::getCamera() {
    return _camera;
}

JS::ClientScriptLoader &GameScene::getScriptLoader() {
    return _scriptLoader;
}
} // namespace Scenes