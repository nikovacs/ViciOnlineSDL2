#pragma once

#include <SDL2/SDL.h>
#include "../ViciEngine/base64.hpp"
#include <string>
#include <string_view>
#include <nlohmann/json.hpp>
#include "UdpClient.hpp"
#include "../ViciEngine/UdpChannels.hpp"
#include <enet/enet.h>

namespace Levels {
	class Level {
	public:
		Level(std::string_view name);
		virtual void update() = 0;
		virtual void render(SDL_Renderer* renderer) = 0;
		int getRenderDistance();
		int getTileSize();
		virtual int getLevelHeight();
		virtual int getLevelWidth();
	protected:
		int _renderDistance{ 0 };
		int _tileSize{};
		std::pair<int, int> _levelDimensions{};
		std::string _name{};
		std::string _currentLevel{}; // same as _name if _name is .vlvl

		void setCurrentLevel(std::string_view level) {
			if (level == _currentLevel) return;
			_currentLevel = level;
			nlohmann::json json{};
			json["lvl"] = level;
			Networking::UdpClient::sendJson(json, Networking::UdpChannels::UpdatePlayerLevel, ENET_PACKET_FLAG_RELIABLE);
		}
		
		inline std::string intToB64(int input) {
			std::string b64Chars{ base64::get_base64_chars() };
			char first{ b64Chars[input / 64] };
			char second{ b64Chars[input % 64] };
			return std::string(1, first) + std::string(1, second);
		}
		inline int b64ToInt(std::string const& input) {
			std::string b64Chars{ base64::get_base64_chars() };
			int total = 0;
			for (int i{ 0 }; i < 2; i++) {
				if (!i) total += 64 * static_cast<int>(b64Chars.find(input.at(i)));
				else total += static_cast<int>(b64Chars.find(input.at(i)));
			}
			return total;
		}
	};
}