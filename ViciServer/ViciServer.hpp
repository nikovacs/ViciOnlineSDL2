#pragma once
#include <enet/enet.h>
#include <memory>
#include "UdpServer.hpp"
#include <thread>
#include "nlohmann/json.hpp"
#include "ServerScriptLoader.hpp"

class ViciServer {
private:
	nlohmann::json _serverOptions;
	std::unique_ptr<std::thread> _networkThread;
	std::unique_ptr<Networking::UdpServer> _udpServer;
	JS::ServerScriptLoader _scriptLoader{};
	static const int TICKS_PER_SECOND{ 20 };
	bool _running;
	void loadServerOptions();
public:
	static inline ViciServer* instance{ nullptr };
	ViciServer();
	virtual ~ViciServer();
	void start();
	void stop();
	void serverLoop();
	nlohmann::json& getServerOptions();
};