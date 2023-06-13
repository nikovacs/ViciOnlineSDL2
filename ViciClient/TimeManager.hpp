#pragma once
class TimeManager {
public:
	inline static Uint32 delta; // time delta in milliseconds
	static void setDelta(Uint32 val) { delta = val; }
};