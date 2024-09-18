#include <chrono>
#include <thread>
#include <iostream>
#include "header_wrappers/enet_wrapper.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "ViciServer/include/ViciServer.hpp"
#include "ViciServer/include/Networking/UdpServer.hpp"

ViciServer* ViciServer::instance = nullptr;

ViciServer::ViciServer() {
	instance = this;
	_loadServerOptions();
	_initDb();
	_running = false;
	_udpServer = std::make_unique<Networking::UdpServer>(_serverOptions["port"], _serverOptions["maxPlayers"]);
	_scriptLoader = std::make_unique<JS::ServerScriptLoader>();
}

ViciServer::~ViciServer() {
}

void ViciServer::_loadServerOptions() {
	std::ifstream in("serverOptions.json");
	std::stringstream buffer;
	buffer << in.rdbuf();
	if (buffer.str().empty()) {
		std::cerr << "serverOptions.json is either empty or does not exist" << std::endl;
		exit(1);
	}
	_serverOptions = nlohmann::json::parse(buffer.str());
}

void ViciServer::_initDb() {
	nlohmann::json& dbOptions = _serverOptions["db"];

	std::unique_ptr<Vici::DbConnectionPool> dbPool;
	if (dbOptions.contains("url")) {
		std::string url = dbOptions["url"];
		dbPool = std::make_unique<Vici::DbConnectionPool>(url, dbOptions["minConnections"]);
	}
	else {
		std::string host = dbOptions["host"];
		int port = dbOptions["port"];
		std::string user = dbOptions["user"];
		std::string password = dbOptions["password"];
		std::string database = dbOptions["database"];
		int minConnections = dbOptions["minConnections"];
		dbPool = std::make_unique<Vici::DbConnectionPool>(host, port, database, user, password, minConnections);
	}
	_dbAsyncQueryRunner = std::make_unique<Vici::DbAsyncQueryRunner>(std::move(dbPool));
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
		_dbAsyncQueryRunner->processInProgress();
		_scriptLoader->update();
		//std::cout << "server loop\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / TICKS_PER_SECOND));
	}
}

Vici::DbAsyncQueryRunner& ViciServer::getDbAsyncQueryRunner() {
	return *_dbAsyncQueryRunner;
}

JS::ServerScriptLoader& ViciServer::getScriptLoader() {
	return *_scriptLoader;
}

v8::Isolate* ViciServer::getIsolate() {
	return _scriptLoader->getIsolate();
}