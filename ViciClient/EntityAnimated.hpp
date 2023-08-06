#pragma once

#include "EntityDrawable.hpp"
#include "Animation.hpp"
#include "NetworkAsset.hpp" // circular inclusion?
#include <memory>
#include <string>
#include <string_view>

namespace Entities {
	class EntityAnimated : public EntityDrawable {
	public:
		EntityAnimated(std::string_view aniName, int x, int y, int direction);
		void render(SDL_Renderer* renderer) override;
		void update();
		void setDir(int dir) override;
		void setAniSoft(std::string_view aniName);
		virtual void setAniHard(std::string_view aniName);
		inline std::string getAni() { return _animationName; };
	protected:
		std::string _animationName{};
		std::unique_ptr<Networking::NetworkAsset<Animations::Animation>> _animation{};
	
	};
}