#include "ViciClient/include/Entities/EntityDrawable.hpp"

namespace Entities {
EntityDrawable::EntityDrawable(std::unique_ptr<nlohmann::json> clientW, std::unique_ptr<nlohmann::json> clientR) : Entity(std::move(clientW), std::move(clientR)) {}
} // namespace Entities