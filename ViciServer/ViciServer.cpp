#include "ViciServer.hpp"
#include "UdpServer.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <enet/enet.h>
#include "nlohmann/json.hpp"
#include <fstream>

ViciServer::ViciServer() {
	_loadServerOptions();
	_running = false;
	_udpServer = std::make_unique<Networking::UdpServer>(_serverOptions["port"], _serverOptions["maxPlayers"]); 
	instance = this;
}

ViciServer::~ViciServer() {
}

void ViciServer::_loadServerOptions() {
	std::ifstream in("serverOptions.json");
	std::stringstream buffer;
	buffer << in.rdbuf();
	_serverOptions = nlohmann::json::parse(buffer.str());
}

void ViciServer::_initDbPool() {
	nlohmann::json& dbOptions = _serverOptions["db"];
	std::string host = dbOptions["host"];
	std::string user = dbOptions["user"];
	std::string password = dbOptions["password"];
	std::string database = dbOptions["database"];
	int minConnections = dbOptions["minConnections"];
	_dbPool = std::make_unique<Vici::DbConnectionPool>(host, database, user, password, minConnections);
}

nlohmann::json& ViciServer::getServerOptions() {
	return _serverOptions;
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