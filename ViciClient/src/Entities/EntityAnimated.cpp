#include "ViciClient/include/Entities/EntityAnimated.hpp"
#include <string>

namespace Entities {
EntityAnimated::EntityAnimated(std::string_view skeletonName, std::string_view dir, int x, int y,
                               std::unique_ptr<nlohmann::json> clientW, std::unique_ptr<nlohmann::json> clientR)
    : EntityDrawable(std::move(clientW), std::move(clientR)) {
        gottanis::SkeletonCreateOptions opts{
            .name = skeletonName.data(),
            .direction = dir.data(),
        };
    _skeleton = gottanis::Animator::getInstance().createSkeleton(opts);
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

void Entities::EntityAnimated::render([[maybe_unused]] SDL_Renderer *renderer) {
    if (_skeleton) {
        _skeleton->render();
    }
}

void EntityAnimated::setSkeleton(std::string_view skeletonName, nlohmann::json *createOpts) {
    if (createOpts) {
        _skeleton = gottanis::Animator::getInstance().createSkeleton(skeletonName, *createOpts);
    } else {
        _skeleton = gottanis::Animator::getInstance().createSkeleton(skeletonName);
    }
    _skeleton->setPosition(_x, _y);
}

gottanis::Skeleton *EntityAnimated::getSkeleton() {
    return _skeleton;
}
} // namespace Entities
