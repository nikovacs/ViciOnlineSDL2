#pragma once

#include "ViciClient/include/Entities/EntityAnimated.hpp"
#include <memory>

namespace Entities {
class NetworkedPlayer : public EntityAnimated {
public:
    NetworkedPlayer(std::string_view username, std::string_view aniName, std::string_view direction, int x, int y,
                    std::unique_ptr<nlohmann::json> clientW, std::unique_ptr<nlohmann::json> clientR);
    virtual ~NetworkedPlayer() = default;
    std::string_view getUsername();

private:
    std::string _username{};
};
} // namespace Entities