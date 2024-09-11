#pragma once

#include <mutex>

class TryLockGuard {
public:
	TryLockGuard(std::mutex& mtx);
	virtual ~TryLockGuard();
	operator bool() const;

	bool isLocked() const;
private:
	std::mutex& _mtx;
	bool _locked{ false };
};