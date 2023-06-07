#pragma once
class ViciServer {
private:
	static const int TICKS_PER_SECOND{ 20 };
	bool _running;
public:
	ViciServer();
	virtual ~ViciServer();
	void start();
	void serverLoop();
};