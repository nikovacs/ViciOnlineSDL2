#pragma once

#include <string_view>
#include <string>
#include <unordered_map>
#include "header_wrappers/sdl_wrapper.h"
#include <vector>
#include <memory>
#include "ViciClient/include/Animations/IAnimation.hpp"
#include "ViciClient/include/Animations/Frame.hpp"
#include "ViciClient/include/AssetTypes/Texture.hpp"

// forward declaration to prevent circular dependency
namespace Networking {
	template <typename T>
	class NetworkAsset;
}

namespace Animations {
	class Animation : public IAnimation {
	public:
		Animation(std::string_view name, std::string_view source);
		virtual ~Animation();
		void update();
		void render(SDL_Renderer* renderer, int x, int y, int direction);
	private:
		void parseSource(std::string& source);
		void parseSprite(std::string& line);
		void parseSetting(std::string& line);
		void parseEffect(std::string& line);
		void parseAniFragment(std::string& line);
		std::string _name;
		std::unordered_map<std::string, std::unique_ptr<Networking::NetworkAsset<AssetTypes::Texture>>> _fileTextureMap{};
		std::unordered_map<int, std::string> _spriteIndexTextureFileMap{};
		std::unordered_map<int, SDL_Rect> _spriteIndexRectMap{};
		std::vector<Frame> _frames{};
		int _currentFrameIndex{ -1 };
		double _secondsRemainingTilNextFrame{};
		bool _continuous{};
		bool _loop;
		std::string _returnAnimation{};
	};	
}