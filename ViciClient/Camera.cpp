#include "Camera.hpp"
#include "ViciClient.hpp"
#include <SDL2/SDL.h>

#include <iostream>

namespace Client {
	Camera* Camera::instance = nullptr;

	Camera::Camera() {
		instance = this;
	}

	Camera::~Camera() {

	}

	void Camera::initialize() {
		setScale(.5f); // pass this in from server settings
	}

	void Camera::update(Levels::Level& level) {
		if (!_target) return;
		_focusX = _target->getX() + _target->getWidth() / 2;
		_focusY = _target->getY() + _target->getHeight() / 2;
		
		int renderDistance{ level.getRenderDistance() };
		int tileSize{ level.getTileSize() };
		int levelWidth{ level.getLevelWidth() };
		int levelHeight{ level.getLevelHeight() };

		int levelWidthPixels{ levelWidth * tileSize };
		int levelHeightPixels{ levelHeight * tileSize };
		//levelWidthPixels += levelWidthPixels * renderDistance * 2;
		//levelHeightPixels += levelHeightPixels * renderDistance * 2;

		int screenWidth, screenHeight;
		SDL_GetWindowSize(ViciClient::instance->getWindow(), &screenWidth, &screenHeight);

		int levelCenterX{ levelWidthPixels / 2 };
		int levelCenterY{ levelHeightPixels / 2 };

		int screenCenterX{ screenWidth / 2 };
		int screenCenterY{ screenHeight / 2 };

		bool levelFitsHorizontally{ levelWidthPixels * _scale <= screenWidth };
		bool levelFitsVertically{ levelHeightPixels * _scale <= screenHeight };

		_camera.w = static_cast<int>(screenWidth / 2 / _scale) - _target->getWidth() / 2;
		_camera.h = static_cast<int>(screenHeight / 2 / _scale) - _target->getHeight() / 2;

		if (levelFitsHorizontally) {
			_camera.x = 0;
			_offsetX = static_cast<int>(screenCenterX / _scale) - levelCenterX;
		}
		else {
			_offsetX = 0;
			_camera.x = max(_target->getX() - _camera.w, 0);
			if (_camera.x + screenWidth / _scale > levelWidthPixels) {
				_camera.x = levelWidthPixels - static_cast<int>(screenWidth / _scale);
			}
		}

		if (levelFitsVertically) {
			_camera.y = 0;
			_offsetY = static_cast<int>(screenCenterY / _scale) - levelCenterY;
		}
		else {
			_offsetY = 0;
			_camera.y = max(_target->getY() - _camera.h, 0);
			if (_camera.y + screenHeight / _scale > levelHeightPixels) {
				_camera.y = levelHeightPixels - static_cast<int>(screenHeight / _scale);
			}
		}
	}

	int Camera::getX() {
		return _camera.x;
	}

	int Camera::getY() {
		return _camera.y;
	}

	int Camera::getW() {
		return _camera.w;
	}

	int Camera::getH() {
		return _camera.h;
	}

	void Camera::setScale(float scale) {
		SDL_RenderSetScale(ViciClient::instance->getRenderer(), scale, scale);
		_scale = scale;
	}

	float Camera::getScale() {
		return _scale;
	}

	int Camera::getOffsetX() {
		return _offsetX;
	}

	int Camera::getOffsetY() {
		return _offsetY;
	}

	void Camera::setFocusObject(Entities::Entity* target) {
		_target = target;
	}
}