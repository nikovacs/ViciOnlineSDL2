#include "EntityAnimated.hpp"
#include <string>

Entities::EntityAnimated::EntityAnimated(std::string_view aniName, double x, double y, int direction) :
	_animation{ std::make_unique<Networking::NetworkAsset<Animations::Animation>>(aniName) },
	_direction{ direction }, _animationName{ aniName } {
	_x = x;
	_y = y;
}

void Entities::EntityAnimated::update() {
	if (_animation->getValue()) 
		_animation->getValue()->update();
}

void Entities::EntityAnimated::render(SDL_Renderer* renderer) {
	if (_animation->getValue())
		_animation->getValue()->render(renderer, _x, _y, _direction);
}

void Entities::EntityAnimated::setDir(int dir) {
	if (dir < 0 || dir > 3) return;
	_direction = dir;
}

int Entities::EntityAnimated::getDir() {
	return _direction;
}

void Entities::EntityAnimated::setAniSoft(std::string_view aniName) {
	if (_animationName == aniName) return;
	setAniHard(aniName);
}

void Entities::EntityAnimated::setAniHard(std::string_view aniName) {
	_animationName = aniName;
	_animation = std::make_unique<Networking::NetworkAsset<Animations::Animation>>(aniName);
}

