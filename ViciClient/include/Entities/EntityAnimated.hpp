#pragma once

#include "ViciClient/include/Animations/IAnimation.hpp"
#include "ViciClient/include/Entities/EntityDrawable.hpp"
#include "ViciClient/include/Networking/NetworkAsset.hpp"
#include <memory>
#include <string>
#include <string_view>

namespace Entities {
class EntityAnimated : public EntityDrawable {
public:
    EntityAnimated(std::string_view aniName, int x, int y, int direction, std::unique_ptr<nlohmann::json> clientW, std::unique_ptr<nlohmann::json> clientR);
    void render(SDL_Renderer *renderer) override;
    void update();
    void setAniSoft(std::string_view aniName);
    virtual void setAniHard(std::string_view aniName);
    std::string_view getAni();

protected:
    std::string _animationName{};
    std::unique_ptr<Networking::NetworkAsset<Animations::IAnimation>> _animation{};
};
} // namespace Entities