#include "Attributes.hpp"
#include <iostream>
Attributes::Attributes(std::string& jsonDump) : _attributes{ jsonDump } {}

Attributes::Attributes(nlohmann::json& json) : _attributes( json ) {} // initializer list causes empty jsons to be arrays...

nlohmann::json& Attributes::getUnderlyingJson() {
	return _attributes;
}

void Attributes::setOnSetAttribCallback(callbackFunc callback) {
	_onSetAttribCallback = callback;
}

void Attributes::setOnGetAttribCallback(callbackFunc callback) {
	_onGetAttribCallback = callback;
}