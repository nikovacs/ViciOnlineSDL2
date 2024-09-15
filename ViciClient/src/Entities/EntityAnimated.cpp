#include "ViciClient/include/Entities/EntityAnimated.hpp"
#include <string>

namespace Entities {
EntityAnimated::EntityAnimated(std::string_view skeletonName, std::string_view dir, int x, int y,
                               std::unique_ptr<nlohmann::json> clientW, std::unique_ptr<nlohmann::json> clientR)
    : EntityDrawable(std::move(clientW), std::move(clientR)) {
    _skeleton = gottanis::Animator::getInstance().createSkeleton(skeletonName.data(), dir.data());
    setPosition(x, y);
}

void EntityAnimated::setPosition(int x, int y) {
    EntityDrawable::setPosition(x, y);
    if (_skeleton) {
        _skeleton->setPosition(_x, _y);
    }
}

void EntityAnimated::update() {
    // gottanis::Skeleton are updated by the gottanis::Animator
}

void Entities::EntityAnimated::render(SDL_Renderer *renderer) {
    if (_skeleton) {
        _skeleton->render();
    }
}

void EntityAnimated::setSkeleton(std::string_view skeletonName, std::string_view dir) {
    if (_skeleton->getName() == skeletonName && _skeleton->getDirection() == dir) {
        return;
    }

    gottanis::SkeletonCreateOptions options;

    if (dir == "") {
        options = {
            .name = skeletonName.data(),
        };
    } else {
        options = {
            .name = skeletonName.data(),
            .direction = dir.data(),
        };
    }

    if (_skeleton) {
        _skeleton->destroy();
        _skeleton = nullptr;
    }

    _skeleton = gottanis::Animator::getInstance().createSkeleton(options);
}

gottanis::Skeleton *EntityAnimated::getSkeleton() {
    return _skeleton;
}
} // namespace Entities
