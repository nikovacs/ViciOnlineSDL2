#pragma once

#include <SDL2/SDL.h>
#include "ViciCore/include/Entities/Entity.hpp"
#include "ViciClient/include/Levels/Level.hpp"

namespace Client {
	class Camera {
	public:
		Camera();
		virtual ~Camera();
		void initialize();
		void update(Levels::Level& level);
		int getX();
		int getY();
		int getW();
		int getH();
		void setScale(float scale, bool notifyServer = true);
		float getScale();
		int getOffsetX();
		int getOffsetY();
		void setFocusObject(Entities::Entity* target);
		inline std::pair<int, int> getFocusPoint() { return std::make_pair(_focusX, _focusY); }
		static Camera* instance;
		
	private:
		SDL_Rect _camera{};
		float _scale{};
		int _offsetX{};
		int _offsetY{};
		int _focusX{};
		int _focusY{};
		Entities::Entity* _target{ nullptr };
	};
}