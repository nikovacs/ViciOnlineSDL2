#include "ViciServer.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <enet/enet.h>

#ifdef _WIN32
// Windows-specfic requirements for enet
#pragma comment(lib, "WS2_32")
#pragma comment(lib, "winmm")
#endif // _WIN32

ViciServer::ViciServer() {
	_running = false;
}

ViciServer::~ViciServer() {
	enet_host_destroy(host);
	enet_deinitialize();
}

void ViciServer::start() {
	_running = true;
	serverLoop();
}

void ViciServer::stop() {
	_running = false;
}

void ViciServer::serverLoop() {
	while (_running) {
		// do stuff
		std::cout << "server loop\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / TICKS_PER_SECOND));
	}
}

void ViciServer::setupNetworking() {
	// implementation taken from tutorial at enet.bespin.org/Tutorial.html
	if (enet_initialize() != 0) {
		std::cout << "enet init failed\n";
		atexit(enet_deinitialize);
	}
	address = { ENET_HOST_ANY, 8424 }; // TODO ServerOptions port
	host = enet_host_create(&address, 1000, 2, 0, 0); // TODO ServerOptions max player count. Have a channel for each type of message? -> level, img, ani, other, whatever

	if (host == NULL) {
		std::cout << "enet host create failed\n";
		exit(EXIT_FAILURE);
	}
}