#pragma once
#include <nlohmann/json.hpp>
#include <string_view>
#include <functional>

using callbackFunc = std::function<void(std::string_view)>;

class Attributes {
public:
	Attributes();
	Attributes(nlohmann::json* json);
	virtual ~Attributes();
	nlohmann::json* getUnderlyingJson();
	void set(std::string_view key, auto value) {
		(*_attributes)[key.data()] = value;
		if (_onSetAttribCallback) _onSetAttribCallback(key);
	}
	nlohmann::json& get(std::string_view key);
	void remove(std::string_view key);
	bool has(std::string_view key);
	void setOnSetAttribCallback(callbackFunc callback);
	void setOnGetAttribCallback(callbackFunc callback);
	void setOnRemoveAttribCallback(callbackFunc callback);
private:
	nlohmann::json* _attributes{};
	callbackFunc _onSetAttribCallback{ nullptr };
	callbackFunc _onGetAttribCallback{ nullptr };
	callbackFunc _onRemoveAttribCallback{ nullptr };
};