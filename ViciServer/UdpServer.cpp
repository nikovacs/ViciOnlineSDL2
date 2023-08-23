#include "UdpServer.hpp"
#include "AssetBroker.hpp"
#include "../ViciEngine/UdpChannels.hpp"
#include <enet/enet.h>
#include <iostream>
#include "ServerPlayerManager.hpp"

Networking::UdpServer::UdpServer(int port, int maxPlayers) : UdpHost(true, maxPlayers, port, UdpChannels::MAX_CHANNELS) {
	AssetBroker::initializeIndex();
}

Networking::UdpServer::~UdpServer() {
}

void Networking::UdpServer::sendJson(ENetPeer* peer, nlohmann::json& json, Networking::UdpChannels channel, ENetPacketFlag flag) {
	auto jsonString = prepareJsonForSending(json);
	ENetPacket* packet = enet_packet_create(jsonString.c_str(), jsonString.length() + 1, flag);
	enet_peer_send(peer, channel, packet);
}

void Networking::UdpServer::doNetworkLoop(ENetHost* server) {
    std::unordered_map<ENetPeer*, uint32_t> peerToPlayerId{}; // needed bc after disconnect connectId is 0
    ENetEvent event;
    while (_isRunning) {
        //std::cout << "udpserver running\n";
        enet_host_service(server, &event, 1000);
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            peerToPlayerId.emplace(event.peer, event.peer->connectID);
            Networking::ServerPlayerManager::sendInitialPlayerData(event.peer);
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            switch (static_cast<int>(event.channelID)) {
            case UdpChannels::Animation:
            case UdpChannels::Texture:
            case UdpChannels::Script:
            case UdpChannels::Level:
                AssetBroker::sendFile(event);
                break;
            case UdpChannels::UpdatePlayerPos:
            {
                nlohmann::json json{ getJsonFromPacket(event.packet) };
                Networking::ServerPlayerManager::updatePlayerPos(event.peer->connectID, json);
                break;
            }
            case UdpChannels::UpdatePlayerAni:
            {
                nlohmann::json json{ getJsonFromPacket(event.packet) };
                Networking::ServerPlayerManager::updatePlayerAni(event.peer->connectID, json);
                break;
            }
            case UdpChannels::UpdatePlayerDir:
            {
                nlohmann::json json{ getJsonFromPacket(event.packet) };
                Networking::ServerPlayerManager::updatePlayerDir(event.peer->connectID, json);
                break;
            }
            case UdpChannels::UpdatePlayerLevel:
            {
                nlohmann::json json{ getJsonFromPacket(event.packet) };
				Networking::ServerPlayerManager::updatePlayerLevel(event.peer->connectID, json);
            }
            case UdpChannels::StartWatchingLevel:
            {
                nlohmann::json json{ getJsonFromPacket(event.packet) };
                Networking::ServerPlayerManager::startWatchingLevel(event.peer->connectID, json);
                break;
            }
            case UdpChannels::StopWatchingLevel:
            {
                nlohmann::json json{ getJsonFromPacket(event.packet) };
                Networking::ServerPlayerManager::stopWatchingLevel(event.peer->connectID, json);
                break;
            }
            }
            enet_packet_destroy(event.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            if (!peerToPlayerId.contains(event.peer)) break;
			Networking::ServerPlayerManager::onPlayerDisconnect(peerToPlayerId.at(event.peer));
			peerToPlayerId.erase(event.peer);
            event.peer->data = NULL;
            break;
        default:
            break;
        }
        enet_host_flush(server);
    }
}