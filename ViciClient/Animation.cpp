#include "Animation.hpp"
#include "TimeManager.hpp"
#include "NetworkAsset.hpp"
#include <regex>
#include <string_view>
#include <string>
#include <vector>
#include <sstream>
#include <SDL2/SDL.h>
#include "Frame.hpp"
#include <ctre.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>

Animations::Animation::Animation(std::string_view name, std::string_view source) : _name{ name } {
	std::string sourceString{ source };
	parseSource(sourceString);
}

Animations::Animation::~Animation() {}

void Animations::Animation::update() {
	if (_currentFrameIndex >= 0 && _frames.size() <= _currentFrameIndex) return;
	
	if (_currentFrameIndex == -1) {
		++_currentFrameIndex;
		_secondsRemainingTilNextFrame = _frames[_currentFrameIndex].getDuration();
		return;
	}
	
	_secondsRemainingTilNextFrame -= TimeManager::getDeltaSeconds();
	if (_secondsRemainingTilNextFrame <= 0) { // if time to change frame
		if (_currentFrameIndex == _frames.size() - 1) { // if is last frame
			if (_continuous) {
				_secondsRemainingTilNextFrame = _frames[_currentFrameIndex].getDuration(); // set time so it does not approach negative infinity
			}
			else if (_loop) {
				_currentFrameIndex = 0;
				_secondsRemainingTilNextFrame = _frames[_currentFrameIndex].getDuration();
			}
		}
		else { // not last frame
			++_currentFrameIndex;
			_secondsRemainingTilNextFrame = _frames[_currentFrameIndex].getDuration();
		}
	}
}

void Animations::Animation::render(SDL_Renderer* renderer, double x, double y, int direction) {
	if (_currentFrameIndex == -1) return;
	if (direction < 0 || direction > 3) return;
	if (_currentFrameIndex >= 0 && _frames.size() <= _currentFrameIndex) return;
	
	Frame& frame{ _frames[_currentFrameIndex] };
	std::vector<int>& spriteIndices{ frame.getSpriteIndicies(direction) };
	std::vector<int>& spriteXs{ frame.getSpriteXs(direction) };
	std::vector<int>& spriteYs{ frame.getSpriteYs(direction) };

	for (int i{ 0 }; i < spriteIndices.size(); i++) {
		int spriteIndex = spriteIndices[i];
		std::string& textureFile{ _spriteIndexTextureFileMap[spriteIndex] };

		AssetTypes::Texture* texture{ _fileTextureMap[textureFile]->getValue() };
		if (!texture) continue;
		
		int spriteX = spriteXs[i];
		int spriteY = spriteYs[i];

		SDL_Rect& src{ _spriteIndexRectMap[spriteIndex] };
		SDL_Rect dst{ spriteX, spriteY, src.w, src.h };

		SDL_RenderCopy(renderer, texture->getUnderlyingTexture(), &src, &dst);
	}
}

void Animations::Animation::parseSource(std::string& source) {
	bool recordSprites{ false };
	bool recordSettings{ false };
	bool recordEffects{ false };
	bool recordAni{ false };
	
	// split on new line character and parse each line
	std::stringstream ss{ source };
	std::string line{};
	while (std::getline(ss, line, '\n')) {
		if (boost::contains(line, "stopsprites")) recordSprites = false;
		else if (boost::contains(line, "stopsettings")) recordSettings = false;
		else if (boost::contains(line, "stopeffects")) recordEffects = false;
		else if (boost::contains(line, "stopani")) recordAni = false;

		boost::trim(line);
		if (line.empty()) continue;

		if (recordSprites) parseSprite(line);
		else if (recordSettings) parseSetting(line);
		else if (recordEffects) parseEffect(line);
		else if (recordAni) parseAniFragment(line);
		
		if (boost::contains(line, "startsprites")) recordSprites = true;
		else if (boost::contains(line, "startsettings")) recordSettings = true;
		else if (boost::contains(line, "starteffects")) recordEffects = true;
		else if (boost::contains(line, "startani")) recordAni = true;
	}
}

void Animations::Animation::parseSprite(std::string& line) {
	int spriteIndex{};
	std::string textureFile{};
	int x{};
	int y{};
	int w{};
	int h{};
	std::string description{};

	int lineIndex{ 0 };
	std::istringstream iss(line);
	std::string tempLine{};
	
	while (std::getline(iss, tempLine, ' ')) {
		switch (lineIndex) {
		case 0:
			spriteIndex = std::stoi(tempLine);
			break;
		case 1:
			textureFile = tempLine;
			break;
		case 2:
			x = std::stoi(tempLine);
			break;
		case 3:
			y = std::stoi(tempLine);
			break;
		case 4:
			w = std::stoi(tempLine);
			break;
		case 5:
			h = std::stoi(tempLine);
			break;
		default:
			break;
		}
		++lineIndex;
	}
	if (_fileTextureMap.count(textureFile) == 0)
		_fileTextureMap[textureFile] = std::make_unique<Networking::NetworkAsset<AssetTypes::Texture>>(textureFile);
			
	_spriteIndexTextureFileMap.emplace(spriteIndex, textureFile);
	_spriteIndexRectMap.emplace(spriteIndex, SDL_Rect{ x, y, w, h });
}

void Animations::Animation::parseSetting(std::string& line) {
	if (line.starts_with("continuous")) {
		_continuous = true;
	}
	else if (line.starts_with("loop")) {
		_loop = true;
	}
	else if (line.starts_with("return")) {
		std::istringstream iss(line);
		std::getline(iss, _returnAnimation, ' ');
		std::getline(iss, _returnAnimation, ' ');
	}
}

void Animations::Animation::parseEffect(std::string& line) {}

void Animations::Animation::parseAniFragment(std::string& line) {
	if (line.starts_with('d') || line.starts_with('a')) {
		_frames.emplace_back();
		_frames.back().pushFrameDirection(line);
	}
	else if (line.starts_with("wait")) {
		_frames.back().setDuration(std::stod(line.substr(5)));
	}
	else if (line.starts_with("audio")) {
		// todo
	}
	else {
		_frames.back().pushFrameDirection(line);
	}
}
