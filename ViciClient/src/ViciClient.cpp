#include "ViciClient/include/ViciClient.hpp"
#include "ViciClient/include/Handlers/KeyboardInputHandler.hpp"
#include "ViciClient/include/Handlers/MouseInputHandler.hpp"
#include "ViciClient/include/Networking/AssetManager.hpp"
#include "ViciClient/include/Networking/UdpClient.hpp"
#include "ViciClient/include/Scenes/Scene.hpp"
#include "header_wrappers/sdl_wrapper.h"
#include <iostream>
#include "header_wrappers/playfab_wrapper.h"
#include <thread>

ViciClient::ViciClient(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
    : _sceneManager{std::make_unique<Scenes::SceneManager>()}, _isRunning{false}, _window{nullptr}, _renderer{nullptr} {
    instance = this;

    int flags{0};
    flags |= fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return;
    }
    _window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);

    Networking::AssetManager::generatePermanentAssets();
    GUI::ViciGuiHost::initialize(_window, _renderer);

    _sceneManager->newLoginScene(
        [this]() -> void {
            _sceneManager->newGameScene();
        });
    _sceneManager->setScene("LoginScene");
}

ViciClient::~ViciClient() {
    // scene manager needs to dispose of rmlContexts before ViciGuiHost deinitializes
    _sceneManager.reset();
    GUI::ViciGuiHost::deinitialize();
    // Clearing the cache is required before deleting the renderer
    // otherwise a debug assert may, or may not, be triggered
    Networking::AssetManager::clearCache();
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
    SDL_Quit();
}

void ViciClient::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            stop();
            break;
        case SDL_KEYDOWN:
            Handlers::KeyboardInputHandler::handleKeyDown(event);
            break;
        case SDL_KEYUP:
            Handlers::KeyboardInputHandler::handleKeyUp(event);
            break;
        case SDL_MOUSEBUTTONDOWN:
            Handlers::MouseInputHandler::handleMouseDown(event);
            break;
        case SDL_MOUSEBUTTONUP:
            Handlers::MouseInputHandler::handleMouseUp(event);
            break;
        default:
            break;
        }

        _sceneManager->handleEvents(&event);
    }
}

void ViciClient::initialize() {
    PlayFab::PlayFabSettings::staticSettings->titleId = "E76BA";
}

void ViciClient::update() {
    _sceneManager->update();
}

void ViciClient::render() {
    SDL_RenderClear(_renderer);
    // do rendering here
    _sceneManager->render(_renderer);

    SDL_RenderPresent(_renderer);
}

void ViciClient::start() {
    _isRunning = true;
}

void ViciClient::stop() {
    _isRunning = false;
}