#include "UdpServer.hpp"
#include "AssetBroker.hpp"
#include "../ViciEngine/UdpChannels.hpp"
#include <enet/enet.h>
#include <iostream>
#include "PlayerManager.hpp"

Networking::UdpServer::UdpServer(int port, int maxPlayers) : UdpHost(true, maxPlayers, port, UdpChannels::MAX_CHANNELS) {
	AssetBroker::initializeIndex();
}

Networking::UdpServer::~UdpServer() {
}

void Networking::UdpServer::doNetworkLoop(ENetHost* server) {
    ENetEvent event;
    while (_isRunning) {
        //std::cout << "udpserver running\n";
        enet_host_service(server, &event, 1000);
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
			Networking::PlayerManager::spawnPlayer(event.peer->connectID);
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            switch (static_cast<int>(event.channelID)) {
            case UdpChannels::Animation:
            case UdpChannels::Texture:
            case UdpChannels::Script:
            case UdpChannels::Level:
                AssetBroker::sendFile(event);
                break;
            }
            
            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
			Networking::PlayerManager::despawnPlayer(event.peer->connectID);
            event.peer->data = NULL;
        default:
            continue;
        }
    }
}