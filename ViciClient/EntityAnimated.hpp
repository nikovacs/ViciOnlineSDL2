#pragma once

#include "EntityDrawable.hpp"
#include "IAnimation.hpp"
#include "NetworkAsset.hpp" // circular inclusion?
#include <memory>
#include <string>
#include <string_view>

namespace Entities {
	class EntityAnimated : public EntityDrawable {
	public:
		EntityAnimated(std::string_view aniName, int x, int y, int direction, nlohmann::json* clientW, nlohmann::json* clientR);
		void render(SDL_Renderer* renderer) override;
		void update();
		void setAniSoft(std::string_view aniName);
		virtual void setAniHard(std::string_view aniName);
		std::string_view getAni();
	protected:
		std::string _animationName{};
		std::unique_ptr<Networking::NetworkAsset<Animations::IAnimation>> _animation{};
	
	};
}