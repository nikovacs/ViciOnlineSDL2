#pragma once

#include "ViciClient/include/Entities/EntityDrawable.hpp"
#include <gottanis/include/Skeleton.h>
#include <memory>
#include <string>
#include <string_view>

namespace Entities {
class EntityAnimated : public EntityDrawable {
public:
    EntityAnimated(std::string_view skeletonName, std::string_view dir, int x, int y, std::unique_ptr<nlohmann::json> clientW, std::unique_ptr<nlohmann::json> clientR);
    virtual void setPosition(int x, int y) override;
    void render(SDL_Renderer *renderer) override;
    void update();
    void setSkeleton(std::string_view skeletonName, std::string_view dir = "");
    gottanis::Skeleton *getSkeleton();
protected:
    gottanis::Skeleton *_skeleton{nullptr}; // Skeletons are owned by the gottanis::Animator
};
} // namespace Entities