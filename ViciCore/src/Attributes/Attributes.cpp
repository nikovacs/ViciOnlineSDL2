#include "ViciCore/include/Attributes/Attributes.hpp"

Attributes::Attributes(std::unique_ptr<nlohmann::json> json) : _attributes(std::move(json)) {} // initializer list causes empty jsons to be arrays...

Attributes::Attributes() : _attributes(new nlohmann::json()) {}

nlohmann::json *Attributes::getUnderlyingJson() {
    return _attributes.get();
}

nlohmann::json &Attributes::get(std::string_view key) {
    if (_onGetAttribCallback)
        _onGetAttribCallback(key);
    return (*_attributes)[key.data()];
}

void Attributes::remove(std::string_view key) {
    _attributes->erase(key.data());
}

bool Attributes::has(std::string_view key) {
    return _attributes->contains(key.data());
}

void Attributes::setOnSetAttribCallback(callbackFunc callback) {
    _onSetAttribCallback = callback;
}

void Attributes::setOnGetAttribCallback(callbackFunc callback) {
    _onGetAttribCallback = callback;
}

void Attributes::setOnRemoveAttribCallback(callbackFunc callback) {
    _onRemoveAttribCallback = callback;
}