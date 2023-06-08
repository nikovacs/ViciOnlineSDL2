#pragma once
#include <enet/enet.h>

class ViciServer {
private:
	static inline ENetHost* host;
	static inline ENetAddress address;
	static const int TICKS_PER_SECOND{ 20 };
	static inline bool _running;
public:
	ViciServer();
	virtual ~ViciServer();
	static void start();
	static void stop();
	static void serverLoop();
	static void setupNetworking();
};