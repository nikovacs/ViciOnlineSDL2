#include "ViciServer.h"
#include <chrono>
#include <thread>

ViciServer::ViciServer() : _running{ false } {
	
}

ViciServer::~ViciServer() {
	
}

void ViciServer::start() {
	_running = true;
	serverLoop();
}

void ViciServer::serverLoop() {
	while (_running) {
		// do stuff
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / TICKS_PER_SECOND));
	}
}