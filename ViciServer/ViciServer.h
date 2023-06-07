#pragma once
class ViciServer {
private:
	bool _started;
public:
	ViciServer();
	virtual ~ViciServer();
	void start();
};