#pragma once

#include <unordered_map>
#include <memory>
#include <chrono>

template <typename kT, typename vT>
class TimedCache : public std::unordered_map<kT, std::shared_ptr<vT>> {
public:
	TimedCache(uint32_t timeToKeepSeconds = 120) : _timeToKeepSeconds{ timeToKeepSeconds } {}
	void update() {
		std::chrono::duration<double> deltaSeconds{ std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - lastTimePoint) };
		double deltaSecondsDouble = deltaSeconds.count();

		for (const auto& pair : *this) {
			if (pair.second.use_count() == 1) {
				_secondsRemainingMap.contains(pair.first) ? _secondsRemainingMap[pair.first] -= deltaSecondsDouble : _secondsRemainingMap[pair.first] = _timeToKeepSeconds;
				if (_secondsRemainingMap[pair.first] <= 0) {
					_secondsRemainingMap.erase(pair.first);
					this->erase(pair.first);
				}
			} else {
				_secondsRemainingMap.erase(pair.first);
			}
		}	
	}
private:
	std::chrono::high_resolution_clock::time_point lastTimePoint{ std::chrono::high_resolution_clock::now() };
	uint32_t _timeToKeepSeconds{};
	std::unordered_map<kT, double> _secondsRemainingMap{};
};