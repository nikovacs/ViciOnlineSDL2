#include "ViciClient/include/Entities/EntityAnimated.hpp"
#include <string>

Entities::EntityAnimated::EntityAnimated(std::string_view aniName, int x, int y, int direction,
                                         std::unique_ptr<nlohmann::json> clientW, std::unique_ptr<nlohmann::json> clientR)
    : EntityDrawable(std::move(clientW), std::move(clientR)),
      _animationName{aniName}, _animation{std::make_unique<Networking::NetworkAsset<Animations::IAnimation>>(aniName)} {
    _x = x;
    _y = y;
    _dir = direction;
}

void Entities::EntityAnimated::update() {
    // gottanis::Skeleton are updated 
}

void Entities::EntityAnimated::render(SDL_Renderer *renderer) {
    // if (_animation->getValue())
    //     _animation->getValue()->render(renderer, _x, _y, _dir);
    _skeleton->render();
}

void Entities::EntityAnimated::setAniSoft(std::string_view aniName) {
    // if (_animationName == aniName)
    //     return;
    // setAniHard(aniName);
}

void Entities::EntityAnimated::setAniHard(std::string_view aniName) {
    // _animationName = aniName;
    // _animation = std::make_unique<Networking::NetworkAsset<Animations::IAnimation>>(aniName);
}

std::string_view Entities::EntityAnimated::getAni() {
    // return _animationName;
}
