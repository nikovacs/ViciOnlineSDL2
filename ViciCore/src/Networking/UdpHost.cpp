#include <iostream>
#include <enet/enet.h>
#include "ViciCore/include/Networking/UdpHost.hpp"
#include "ViciCore/include/Networking/UdpChannels.hpp"

Networking::UdpHost::UdpHost(bool isServer, int maxConnections, int port, int numChannels) {
	// implementation taken from tutorial at enet.bespin.org/Tutorial.html
	if (enet_initialize() != 0) {
		atexit(enet_deinitialize);
	}
	_address = { ENET_HOST_ANY, static_cast<unsigned short>(port)};
	_host = enet_host_create(isServer ? &_address : NULL, maxConnections, numChannels, 0, 0);

	if (_host == NULL) {
		exit(EXIT_FAILURE);
	}

	instance = this;
}

Networking::UdpHost::~UdpHost() {
	enet_host_destroy(_host);
}

void Networking::UdpHost::start() {
	std::cout << "starting udphost\n";
	_isRunning = true;
	doNetworkLoop(_host);
}

void Networking::UdpHost::stop() {
	_isRunning = false;
}

ENetHost* Networking::UdpHost::getHost() {
	return _host;
}