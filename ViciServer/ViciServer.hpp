#pragma once
#include <enet/enet.h>
#include <memory>
#include "UdpServer.hpp"
#include <thread>

class ViciServer {
private:
	std::unique_ptr<std::thread> _networkThread;
	std::unique_ptr<Networking::UdpServer> _udpServer;
	static const int TICKS_PER_SECOND{ 20 };
	bool _running;
public:
	static inline ViciServer* instance{ nullptr };
	ViciServer();
	virtual ~ViciServer();
	void start();
	void stop();
	void serverLoop();
};