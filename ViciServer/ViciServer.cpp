#include "ViciServer.h"
#include "UdpServer.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <enet/enet.h>

ViciServer::ViciServer() {
	_running = false;
	_udpServer = std::make_unique<Networking::UdpServer>(8424, 1000); // make these number 

	instance = this;
}

ViciServer::~ViciServer() {
}

void ViciServer::start() {
	_running = true;
	_networkThread = std::make_unique<std::thread>(&Networking::UdpServer::start, _udpServer.get());
	serverLoop();
}

void ViciServer::stop() {
	_running = false;
	_udpServer->stop();
	_networkThread->join();
}

void ViciServer::serverLoop() {
	while (_running) {
		// do stuff
		//std::cout << "server loop\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / TICKS_PER_SECOND));
	}
}