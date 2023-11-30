#pragma once
#include "Attributes.hpp"
#include <nlohmann/json.hpp>

class IHasClientW {
public:
	nlohmann::json& getClientWriteableAttrs() {
		return _clientWriteableAttrs.getUnderlyingJson();
	}
private:
	Attributes _clientWriteableAttrs{};
};