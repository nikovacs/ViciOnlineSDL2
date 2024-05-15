#include "UdpClient.hpp"
#include "AssetManager.hpp"
#include "../ViciEngine/UdpChannels.hpp"
#include <enet/enet.h>
#include <string>
#include <iostream>
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "nlohmann/json.hpp"
#include "ClientPlayerManager.hpp"
#include "GameScene.hpp"
#include "ViciClient.hpp"
#include "PlayerInfo.hpp"

#include <iostream>
namespace Networking {
    UdpClient::UdpClient(const std::string_view url, int port) : UdpHost(false, 1, port, UdpChannels::MAX_CHANNELS) {
        enet_address_set_host(&_address, url.data());
        _gameServer = enet_host_connect(_host, &_address, UdpChannels::MAX_CHANNELS, 0);
        AssetManager::initializeIndex();
    }

    UdpClient::~UdpClient() {
        enet_peer_disconnect(_gameServer, 0);
    }

    ENetPeer* UdpClient::getGameServer() {
        return _gameServer;
    }

    void UdpClient::sendJson(nlohmann::json& json, UdpChannels channel, ENetPacketFlag flag) {
        auto jsonString = prepareJsonForSending(json);
        ENetPacket* packet = enet_packet_create(jsonString.c_str(), jsonString.length() + 1, flag);
        enet_peer_send(static_cast<UdpClient*>(UdpClient::instance)->getGameServer(), channel, packet);
        enet_packet_destroy(packet);
    }

    void UdpClient::sendSimplePacket(SimplePacket& simplePacket, UdpChannels channel, ENetPacketFlag flag) {
        ENetPacket* packet = simplePacket.createEnetPacket(channel, flag);
        enet_peer_send(static_cast<UdpClient*>(UdpClient::instance)->getGameServer(), channel, packet);
    }

    void UdpClient::doNetworkLoop(ENetHost* client) {
        ENetEvent event;
        while (_isRunning) {
            enet_host_service(client, &event, 1); // TODO change last arg
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port << '\n';
                /* Store any relevant client information here. */
                //event.peer->data = "Client information";
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                switch (event.channelID) {
                case UdpChannels::Animation:
                case UdpChannels::Texture:
                case UdpChannels::Script:
                case UdpChannels::Level:
                case UdpChannels::String:
                    AssetManager::onReceived(event);
                    break;
                case UdpChannels::handshake:
                {
                    nlohmann::json handshake{};
                    handshake["usr"] = PlayerInfo::username;
                    handshake["id"] = PlayerInfo::playerId;
                    sendJson(handshake, UdpChannels::handshake, ENET_PACKET_FLAG_RELIABLE);
                }
                break;
                case UdpChannels::initialPlayerData:
                {
                    auto jsonInitPlayerData = getJsonFromPacket(event.packet);
                    reinterpret_cast<Scenes::GameScene*>(&Scenes::SceneManager::instance->getScene("Game"))->loadInitPlayerData(jsonInitPlayerData);
                    Scenes::SceneManager::instance->setScene("Game");
                }
                break;
                case UdpChannels::SpawnPlayer:
                {
                    auto jsonSpawnPlayer = getJsonFromPacket(event.packet);
                    ClientPlayerManager::spawnPlayer(jsonSpawnPlayer);
                }
                break;
                case UdpChannels::DespawnPlayer:
                {
                    auto jsonDespawnPlayer = getJsonFromPacket(event.packet);
                    ClientPlayerManager::despawnPlayer(jsonDespawnPlayer);
                }
                break;
                case UdpChannels::UpdatePlayerPos:
                {
                    auto jsonPlayerPos = getJsonFromPacket(event.packet);
                    ClientPlayerManager::updatePlayerPos(jsonPlayerPos);
                }
                break;
                case UdpChannels::UpdatePlayerDir:
                {
                    auto jsonPlayerDir = getJsonFromPacket(event.packet);
                    ClientPlayerManager::updatePlayerDir(jsonPlayerDir);
                }
                break;
                case UdpChannels::UpdatePlayerAni:
                {
                    auto jsonPlayerAni = getJsonFromPacket(event.packet);
                    ClientPlayerManager::updatePlayerAni(jsonPlayerAni);
                }
                }


                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << event.peer->data << " disconnected." << std::endl;
                /* Reset the peer's client information. */
                event.peer->data = NULL;
            default:
                break;
            }
            enet_host_flush(getHost());
        }
    }
}
