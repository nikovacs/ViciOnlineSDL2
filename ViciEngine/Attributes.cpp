#include "Attributes.hpp"

Attributes::Attributes(std::string& jsonDump) : _attributes{ jsonDump } {}

nlohmann::json& Attributes::getUnderlyingJson() {
	return _attributes;
}

void Attributes::set(std::string_view key, auto value) {
	_attributes[key.data()] = value;
	if (_onSetAttribCallback) _onSetAttribCallback(key);
}

auto Attributes::get(std::string_view key) {
	if (_onGetAttribCallback) _onGetAttribCallback(key);
	return _attributes[key.data()];
}

void Attributes::setOnSetAttribCallback(callbackFunc callback) {
	_onSetAttribCallback = callback;
}

void Attributes::setOnGetAttribCallback(callbackFunc callback) {
	_onGetAttribCallback = callback;
}