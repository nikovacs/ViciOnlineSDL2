#include "UdpServer.hpp"
#include "AssetBroker.hpp"
#include "../ViciEngine/UdpChannels.hpp"
#include <enet/enet.h>
#include <iostream>

Networking::UdpServer::UdpServer(int port, int maxPlayers) : UdpHost(true, maxPlayers, port, UdpChannels::MAX_CHANNELS) {
	AssetBroker::initializeIndex();
}

Networking::UdpServer::~UdpServer() {
}

void Networking::UdpServer::doNetworkLoop(ENetHost* server) {
    ENetEvent event;
    while (_isRunning) {
        std::cout << "udpserver running\n";
        enet_host_service(server, &event, 1000);
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
			std::cout << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port << '\n';
            /* Store any relevant client information here. */
            //event.peer->data = "Client information";
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            std::cout << "A packet of length " 
                << event.packet->dataLength 
                << " containing " << event.packet->data 
                << " was received from " << event.peer->data 
                << " on channel " << int(event.channelID) << '\n';

            switch (static_cast<int>(event.channelID)) {
            case UdpChannels::Animation:
            case UdpChannels::Texture:
                AssetBroker::sendFile(event);
                break;
            }
            
            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
			std::cout << event.peer->data << " disconnected." << std::endl;
            /* Reset the peer's client information. */
            event.peer->data = NULL;
        default:
            continue;
        }
    }
}