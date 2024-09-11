#include "ViciClient/include/Entities/EntityDrawable.hpp"

namespace Entities {
	EntityDrawable::EntityDrawable(nlohmann::json* clientW, nlohmann::json* clientR) : Entity(clientW, clientR) {}
}