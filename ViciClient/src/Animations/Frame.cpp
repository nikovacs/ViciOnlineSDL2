#include <string>
#include <vector>
#include "ViciClient/include/Animations/Frame.hpp"

Animations::Frame::Frame() : _durationSeconds{ 0.05 } {}

Animations::Frame::~Frame() {}

void Animations::Frame::pushFrameDirection(std::string& line) {
	_frameDirections.emplace_back(line);
}

void Animations::Frame::setDuration(double durationSeconds) {
	_durationSeconds = durationSeconds;
}

double Animations::Frame::getDuration() {
	return _durationSeconds;
}

std::vector<int>& Animations::Frame::getSpriteIndicies(int dir) {
	return _frameDirections[dir].spriteIndices;
}

std::vector<int>& Animations::Frame::getSpriteXs(int dir) {
	return _frameDirections[dir].spriteXs;
}

std::vector<int>& Animations::Frame::getSpriteYs(int dir) {
	return _frameDirections[dir].spriteYs;
}