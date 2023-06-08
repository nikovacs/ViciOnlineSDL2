#include "UdpHost.h"
#include "UdpChannels.h"

Networking::UdpHost::UdpHost(int maxConnections, int port, int numChannels) {
	// implementation taken from tutorial at enet.bespin.org/Tutorial.html
	if (enet_initialize() != 0) {
		atexit(enet_deinitialize);
	}
	_address = { ENET_HOST_ANY, static_cast<unsigned short>(port)}; // TODO ServerOptions port
	_host = enet_host_create(&_address, maxConnections, numChannels, 0, 0); // TODO ServerOptions max player count. Have a channel for each type of message? -> level, img, ani, other, whatever

	if (_host == NULL) {
		exit(EXIT_FAILURE);
	}
}

Networking::UdpHost::~UdpHost() {
	enet_host_destroy(_host);
	enet_deinitialize();
}

void Networking::UdpHost::start() {
	_isRunning = true;
	hostLoop();
}

void Networking::UdpHost::stop() {
	_isRunning = false;
}

void Networking::UdpHost::hostLoop() {
	ENetEvent event; // if workload is spread across numerous threads, this will have to be changed to a dynamic number of events
	while (_isRunning) {
		enet_host_service(_host, &event, 1000);
		switch (event.type) {
		case ENET_EVENT_TYPE_NONE:
			continue;
		case ENET_EVENT_TYPE_CONNECT:
			// TODO
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			// TODO
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			// TODO
			break;
		}
	}
}