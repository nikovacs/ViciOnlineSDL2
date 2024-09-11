#pragma once

#include <SDL2/SDL.h>

namespace Animations {
	class IAnimation {
	public:
		virtual ~IAnimation() = default;

		/*
		* Updates the animation
		* 
		* Include TimeManager.hpp for delta time
		*/
		virtual void update() = 0;

		/*
		* Renders the animation at the given position and direction
		* 
		* @param renderer The renderer to render the animation to
		* @param x The entity's x position
		* @param y The entity's y position
		* @param direction The direction the entity is facing (Non-Negative)
		*/
		virtual void render(SDL_Renderer* renderer, int x, int y, int direction) = 0;
	};
}