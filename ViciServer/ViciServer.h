#pragma once

class ViciServer {
private:
	static const int TICKS_PER_SECOND{ 20 };
	static inline bool _running;
public:
	ViciServer();
	virtual ~ViciServer();
	void start();
	void stop();
	static void serverLoop();
};