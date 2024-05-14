#include "TryLockGuard.hpp"

TryLockGuard::TryLockGuard(std::mutex& mtx) : _mtx{ mtx } {
	_locked = _mtx.try_lock();
 }

TryLockGuard::~TryLockGuard() {
	if (_locked) {
		_mtx.unlock();
	}
}

TryLockGuard::operator bool() const {
	return isLocked();
}

bool TryLockGuard::isLocked() const {
	return _locked;
}