#pragma once
#include <enet/enet.h>
#include <memory>
#include <thread>
#include <nlohmann/json.hpp>
#include "../../ViciServer/include/Networking/UdpServer.hpp"
#include "../../ViciServer/include/JSRuntime/ServerScriptLoader.hpp"
#include "../../ViciServer/include/Database/DbAsyncQueryRunner.hpp"

class ViciServer {
private:
	nlohmann::json _serverOptions;
	std::unique_ptr<Vici::DbAsyncQueryRunner> _dbAsyncQueryRunner;
	std::unique_ptr<std::thread> _networkThread;
	std::unique_ptr<Networking::UdpServer> _udpServer;
	std::unique_ptr<JS::ServerScriptLoader> _scriptLoader{ nullptr };
	static const int TICKS_PER_SECOND{ 20 };
	bool _running;
	void _loadServerOptions();
	void _initDb();
public:
	static ViciServer* instance;
	ViciServer();
	virtual ~ViciServer();
	void start();
	void stop();
	void serverLoop();
	nlohmann::json& getServerOptions();
	Vici::DbAsyncQueryRunner& getDbAsyncQueryRunner();
	JS::ServerScriptLoader& getScriptLoader();
	v8::Isolate* getIsolate();
};