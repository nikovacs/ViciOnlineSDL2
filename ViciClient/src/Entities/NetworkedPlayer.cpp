#include "ViciClient/include/Entities/NetworkedPlayer.hpp"
#include "ViciClient/include/Networking/UdpClient.hpp"

namespace Entities {
NetworkedPlayer::NetworkedPlayer(std::string_view username, std::string_view aniName, std::string_view direction, int x, int y,
                                 std::unique_ptr<nlohmann::json> clientW, std::unique_ptr<nlohmann::json> clientR)
    : EntityAnimated(aniName, direction, x, y, std::move(clientW), std::move(clientR)), _username{username} {
    _clientW.setOnSetAttribCallback([this](std::string_view key) -> void {
        nlohmann::json json{};
        json["k"] = key;
        json["v"] = _clientW.get(key);

        Networking::UdpClient::sendJson(json, Networking::UdpChannels::UpdateClientW, ENET_PACKET_FLAG_RELIABLE);
    });
}

std::string_view NetworkedPlayer::getUsername() {
    return _username;
}
} // namespace Entities