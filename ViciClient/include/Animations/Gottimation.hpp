#pragma once

#include <string_view>
#include "ViciClient/include/Animations/IAnimation.hpp"

namespace Animations {
	class Gottimation : public IAnimation {
	public:
		Gottimation(std::string_view name, std::string_view source);
		virtual ~Gottimation() = default;
		void update() override;
		void render(SDL_Renderer* renderer, int x, int y, int direction) override;
	};
}