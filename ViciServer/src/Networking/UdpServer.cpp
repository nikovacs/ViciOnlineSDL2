#include "header_wrappers/enet_wrapper.h"
#include <iostream>
#include "ViciCore/include/Networking/UdpChannels.hpp"
#include "ViciServer/include/ViciServer.hpp"
#include "ViciServer/include/Networking/UdpServer.hpp"
#include "ViciServer/include/Networking/AssetBroker.hpp"
#include "ViciServer/include/Networking/ServerPlayerManager.hpp"
#include "ViciServer/include/JSRuntime/ServerPlayerJSWrapper.hpp"

Networking::UdpServer::UdpServer(int port, int maxPlayers) : UdpHost(true, maxPlayers, port, UdpChannels::MAX_CHANNELS) {
	AssetBroker::initializeIndex();
}

Networking::UdpServer::~UdpServer() {
}

void Networking::UdpServer::sendEmpty(ENetPeer* peer, Networking::UdpChannels channel, ENetPacketFlag flag) {
    ENetPacket* packet = enet_packet_create(NULL, 0, flag);
	int code = enet_peer_send(peer, channel, packet);
	if (code != 0) {
	    enet_packet_destroy(packet);
	}
}

void Networking::UdpServer::sendJson(ENetPeer* peer, nlohmann::json& json, Networking::UdpChannels channel, ENetPacketFlag flag) {
	auto jsonString = prepareJsonForSending(json);
	ENetPacket* packet = enet_packet_create(jsonString.c_str(), jsonString.length() + 1, flag);
	int code = enet_peer_send(peer, channel, packet);
    if (code != 0) {
        enet_packet_destroy(packet);
	}
}

void Networking::UdpServer::sendSimplePacket(ENetPeer* peer, SimplePacket& simplePacket, Networking::UdpChannels channel, ENetPacketFlag flag) {
    ENetPacket* packet = simplePacket.createEnetPacket(channel, flag);
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
        {
            peerToPlayerId.emplace(event.peer, event.peer->connectID);
            sendEmpty(event.peer, UdpChannels::handshake, ENET_PACKET_FLAG_RELIABLE);
            break;
        }
        case ENET_EVENT_TYPE_RECEIVE:
            switch (static_cast<int>(event.channelID)) {
            case UdpChannels::Json:
            case UdpChannels::Texture:
            case UdpChannels::Script:
            case UdpChannels::Level:
            case UdpChannels::String:
            {
                SimplePacket packet{ event.packet };
                AssetBroker::sendFile(event.peer, static_cast<UdpChannels>(event.channelID), packet);
                break;
            }
            case UdpChannels::handshake:
            {
                SimplePacket packet{ event.packet };
                Networking::ServerPlayerManager::sendInitialPlayerData(event.peer, packet);
                break;
            }
            case UdpChannels::connectionAccepted:
            {
                SimplePacket packet{ event.packet };
                ViciServer::instance->getScriptLoader().onPlayerConnect(event.peer->connectID);
                break;
            }
            case UdpChannels::UpdatePlayerPos:
            {
                SimplePacket packet{ event.packet };
                Networking::ServerPlayerManager::updatePlayerPos(event.peer->connectID, packet);
                break;
            }
            // case UdpChannels::UpdatePlayerAni:
            // {
            //     SimplePacket packet{ event.packet };
            //     Networking::ServerPlayerManager::updatePlayerAni(event.peer->connectID, packet);
            //     break;
            // }
            case UdpChannels::UpdatePlayerCameraZoom:
            {
                SimplePacket packet{ event.packet };
                Networking::ServerPlayerManager::updatePlayerCameraZoom(event.peer->connectID, packet);
                break;
            }
            // case UdpChannels::UpdatePlayerDir:
            // {
            //     SimplePacket packet{ event.packet };
            //     Networking::ServerPlayerManager::updatePlayerDir(event.peer->connectID, packet);
            //     break;
            // }
            case UdpChannels::UpdatePlayerLevel:
            {
                SimplePacket packet{ event.packet };
				Networking::ServerPlayerManager::updatePlayerLevel(event.peer->connectID, packet);
                break;
            }
            case UdpChannels::StartWatchingLevel:
            {
                SimplePacket packet{ event.packet };
                Networking::ServerPlayerManager::startWatchingLevel(event.peer->connectID, packet);
                break;
            }
            case UdpChannels::StopWatchingLevel:
            {
                SimplePacket packet{ event.packet };
                Networking::ServerPlayerManager::stopWatchingLevel(event.peer->connectID, packet);
                break;
            }
            case UdpChannels::UpdateClientW:
            {
                nlohmann::json json{ getJsonFromPacket(event.packet) };
				Networking::ServerPlayerManager::updatePlayerClientW(event.peer->connectID, json);
                enet_packet_destroy(event.packet);
				break;
            }
            }
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
			std::cout << "disconnect\n";
            if (!peerToPlayerId.contains(event.peer)) break;
			Networking::ServerPlayerManager::onPlayerDisconnect(peerToPlayerId.at(event.peer));
			peerToPlayerId.erase(event.peer);
            event.peer->data = NULL;
            enet_packet_destroy(event.packet);
            break;
        default:
            enet_packet_destroy(event.packet);
            break;
        }
        enet_host_flush(server);
    }
}