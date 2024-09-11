#pragma once

class TimeManager {
public:
	inline static uint32_t delta; // time delta in milliseconds
	static void setDelta(uint32_t val) { delta = val; }
	static double getDeltaSeconds() { return delta / 1000.0; }
	static double getDeltaMilliseconds() { return delta; }
};