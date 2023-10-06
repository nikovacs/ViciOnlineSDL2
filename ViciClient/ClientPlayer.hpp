#pragma once

#include "EntityAnimated.hpp"
#include "../ViciEngine/Attributes.hpp"
#include <nlohmann/json.hpp>

namespace Entities {
	class ClientPlayer : public EntityAnimated {
	public:
		ClientPlayer(nlohmann::json& json);
		void setDir(int dir) override;
		void setAniHard(std::string_view aniName) override;
		void setPosition(int x, int y) override;
	private:
		Attributes _clientWriteableAttrs{};
	};
}