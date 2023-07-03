#pragma once

#include <string_view>
#include <string>
#include <unordered_map>
#include "NetworkAsset.hpp"
#include "Texture.hpp"
#include <SDL2/SDL.h>
#include "TimeManager.hpp"

namespace AssetTypes {
	class Animation {
	public:
		Animation(std::string_view name, std::string_view source);
		virtual ~Animation();
		void update();
		void render(SDL_Renderer* renderer);
	private:
		void parseSource(std::string_view source);
		std::string _name;
		std::unordered_map<std::string, Networking::NetworkAsset<AssetTypes::Texture>> _fileTextureMap{};
		std::unordered_map<int, std::string> _spriteIndexTextureFileMap{};
		std::unordered_map<int, SDL_Rect> _spriteIndexRectMap{};
		int _currentFrameIndex{};
		double _secondsRemaining{};
		bool _continuous{};
		bool _loop;
		std::string _returnAnimation{};
	};
}