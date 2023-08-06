#include "Entity.hpp"

void Entities::Entity::setPosition(int x, int y) {
	_x = x;
	_y = y;
}

int Entities::Entity::getX() {
	return _x;
}

int Entities::Entity::getY() {
	return _y;
}

int Entities::Entity::getWidth() {
	return _width;
}

void Entities::Entity::setWidth(int w) {
	_width = w;
}

int Entities::Entity::getHeight() {
	return _height;
}

void Entities::Entity::setHeight(int h) {
	_height = h;
}

void Entities::Entity::setDir(int dir) {
	if (dir < 0 || dir > 3) return;
	_dir = dir;
}

int Entities::Entity::getDir() {
	return _dir;
}