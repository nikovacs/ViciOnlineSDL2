#include "ViciClient/include/JSRuntime/ClientPlayerJSWrapper.hpp"
#include "ViciClient/include/PlayerInfo.hpp"
#include "ViciClient/include/ViciClient.hpp"
#include <iostream>

namespace JS {
ClientPlayerJSWrapper::ClientPlayerJSWrapper(Entities::ClientPlayer *pl, v8pp::context *ctx) : AbstractPlayerJSWrapper{ctx, pl}, _player{pl} {
    setAllowWriteClientW(true);
}

std::string ClientPlayerJSWrapper::getUsername() {
    return PlayerInfo::username;
}

void ClientPlayerJSWrapper::setSkeleton(std::string skelName, v8::Local<v8::Value> createOpts) {
    if (!createOpts->IsNullOrUndefined()) {
        std::string createOpts = v8pp::json_str(v8::Isolate::GetCurrent(), createOpts);
        nlohmann::json opts = nlohmann::json::parse(createOpts);
        _player->setSkeleton(skelName, opts);
    } else {
        _player->setSkeleton(skelName);
    }
}

std::string ClientPlayerJSWrapper::getSkeleton() {
    if (_player->getSkeleton()) {
        return _player->getSkeleton()->getName();
    } else {
        return "";
    }
}

// int ClientPlayerJSWrapper::getX() {
// 	return _player->getX();
// }

// void ClientPlayerJSWrapper::setX(int x) {
// 	_player->setPosition(x, _player->getY());
// }

// int ClientPlayerJSWrapper::getY() {
// 	return _player->getY();
// }

// void ClientPlayerJSWrapper::setY(int y) {
// 	_player->setPosition(_player->getX(), y);
// }

// std::string ClientPlayerJSWrapper::getDir() {
// 	return _player->getSkeleton()->getDirection();
// }

// void ClientPlayerJSWrapper::setDir(std::string dir) {
// 	_player->getSkeleton()->setDirection(dir);
// }

// int ClientPlayerJSWrapper::getWidth() {
// 	return _player->getWidth();
// }

// void ClientPlayerJSWrapper::setWidth(int w) {
// 	_player->setWidth(w);
// }

// int ClientPlayerJSWrapper::getHeight() {
// 	return _player->getHeight();
// }

// void ClientPlayerJSWrapper::setHeight(int h) {
// 	_player->setHeight(h);
// }
} // namespace JS