#pragma once
#include <nlohmann/json.hpp>
#include <string_view>
#include <functional>

using callbackFunc = std::function<void(std::string_view)>;

class Attributes {
public:
	Attributes() = default;
	Attributes(std::string& jsonDump);
	Attributes(nlohmann::json& json);

	nlohmann::json& getUnderlyingJson();
	void set(std::string_view key, auto value) {
		_attributes[key.data()] = value;
		if (_onSetAttribCallback) _onSetAttribCallback(key);
	}

	auto get(std::string_view key) {
		if (_onGetAttribCallback) _onGetAttribCallback(key);
		return _attributes[key.data()];
	}

	void setOnSetAttribCallback(callbackFunc callback);
	void setOnGetAttribCallback(callbackFunc callback);
private:
	nlohmann::json _attributes{};
	callbackFunc _onSetAttribCallback{ nullptr };
	callbackFunc _onGetAttribCallback{ nullptr };
};