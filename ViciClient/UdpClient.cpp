#include "UdpClient.hpp"
#include "AssetManager.hpp"
#include "../ViciEngine/UdpChannels.hpp"
#include <enet/enet.h>
#include <string>
#include <iostream>

Networking::UdpClient::UdpClient(const std::string_view url, int port) : UdpHost(false, 1, port, UdpChannels::MAX_CHANNELS) {
	enet_address_set_host(&_address, url.data());
	_gameServer = enet_host_connect(_host, &_address, UdpChannels::MAX_CHANNELS, 0);
	AssetManager::initializeIndex();
}

Networking::UdpClient::~UdpClient() {
}

ENetPeer* Networking::UdpClient::getGameServer() {
	return _gameServer;
}

void Networking::UdpClient::doNetworkLoop(ENetHost* client) {
    ENetEvent event;
    while (_isRunning) {
        //std::cout << "udpclient running\n";
        enet_host_service(client, &event, 1000);
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            std::cout << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port << '\n';
            /* Store any relevant client information here. */
            //event.peer->data = "Client information";
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            /*std::cout << "A packet of length "
                << event.packet->dataLength
                << " containing " << event.packet->data
                << " was received from " << event.peer->data
                << " on channel " << int(event.channelID) << '\n';*/
            
            switch (event.channelID) {
            case UdpChannels::Animation:
			case UdpChannels::Texture:
            case UdpChannels::Script:
            case UdpChannels::Level:
                AssetManager::onReceived(event);
				break;
            }

            
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
