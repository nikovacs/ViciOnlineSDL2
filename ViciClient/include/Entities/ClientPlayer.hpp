#pragma once

#include <nlohmann/json.hpp>
#include "ViciClient/include/Entities/EntityAnimated.hpp"

namespace Entities {
	class ClientPlayer : public EntityAnimated {
	public:
		ClientPlayer(nlohmann::json& json);
		virtual ~ClientPlayer() = default;
		void setDir(int dir) override;
		void setAniHard(std::string_view aniName) override;
		void setPosition(int x, int y) override;
	};
}