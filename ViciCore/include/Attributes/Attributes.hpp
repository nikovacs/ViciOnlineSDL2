#pragma once
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <string_view>

using callbackFunc = std::function<void(std::string_view)>;

class Attributes {
public:
    Attributes();
    Attributes(std::unique_ptr<nlohmann::json> json);
    virtual ~Attributes() = default;
    nlohmann::json *getUnderlyingJson();
    void set(std::string_view key, auto value) {
        if ((*_attributes)[key.data()] == value) {
            return;
        }
        (*_attributes)[key.data()] = value;
        if (_onSetAttribCallback)
            _onSetAttribCallback(key);
    }
    nlohmann::json &get(std::string_view key);
    void remove(std::string_view key);
    bool has(std::string_view key);
    void setOnSetAttribCallback(callbackFunc callback);
    void setOnGetAttribCallback(callbackFunc callback);
    void setOnRemoveAttribCallback(callbackFunc callback);

private:
    std::unique_ptr<nlohmann::json> _attributes{};
    callbackFunc _onSetAttribCallback{nullptr};
    callbackFunc _onGetAttribCallback{nullptr};
    callbackFunc _onRemoveAttribCallback{nullptr};
};