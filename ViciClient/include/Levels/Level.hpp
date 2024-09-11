#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <string_view>
#include <nlohmann/json.hpp>
#include <enet/enet.h>
#include <set>
#include "ViciCore/include/Utility/base64.hpp"
#include "ViciCore/include/Networking/UdpChannels.hpp"
#include "ViciCore/include/Networking/SimplePacket.hpp"
#include "ViciClient/include/Networking/UdpClient.hpp"

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
		virtual void getRenderedLevels(std::set<std::string>& lvlsOut);
	protected:
		int _renderDistance{ 0 };
		int _tileSize{};
		std::pair<int, int> _levelDimensions{};
		std::string _name{};
		std::string _currentLevel{}; // same as _name if _name is .vlvl

		void setCurrentLevel(std::string_view level) {
			if (level == _currentLevel) return;
			_currentLevel = level;

			Networking::SimplePacket packet{};
			packet.add(_currentLevel);
			Networking::UdpClient::sendSimplePacket(packet, Networking::UdpChannels::UpdatePlayerLevel, ENET_PACKET_FLAG_RELIABLE);
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