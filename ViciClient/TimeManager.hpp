#pragma once
class TimeManager {
public:
	inline static Uint32 delta; // time delta in milliseconds
	static void setDelta(Uint32 val) { delta = val; }
	static double getDeltaSeconds() { return delta / 1000.0; }
	static double getDeltaMilliseconds() { return delta; }
};