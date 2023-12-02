#include "Attributes.hpp"
#include <iostream>

Attributes::Attributes(nlohmann::json* json) : _attributes( json ) {} // initializer list causes empty jsons to be arrays...

Attributes::Attributes() : _attributes( new nlohmann::json() ) {}

Attributes::~Attributes() {
	delete _attributes;
}

nlohmann::json* Attributes::getUnderlyingJson() {
	return _attributes;
}

void Attributes::set(std::string_view key, nlohmann::json& value) {
	(*_attributes)[key.data()] = value;
	if (_onSetAttribCallback) _onSetAttribCallback(key);
}

nlohmann::json& Attributes::get(std::string_view key) {
	if (_onGetAttribCallback) _onGetAttribCallback(key);
	return (*_attributes)[key.data()];
}

void Attributes::setOnSetAttribCallback(callbackFunc callback) {
	_onSetAttribCallback = callback;
}

void Attributes::setOnGetAttribCallback(callbackFunc callback) {
	_onGetAttribCallback = callback;
}