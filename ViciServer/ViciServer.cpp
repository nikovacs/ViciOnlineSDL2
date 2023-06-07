#include "ViciServer.h"
#include <chrono>
#include <thread>
#include <iostream>
ViciServer::ViciServer() {
	_running = false;
}

ViciServer::~ViciServer() {
	
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