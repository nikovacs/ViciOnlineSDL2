#include "ViciServer.h"

ViciServer::ViciServer() : _started{ false } {
	
}

ViciServer::~ViciServer() {
	
}

void ViciServer::start() {
	_started = true;
}