#pragma once
#include "ViciCore/include/Attributes/Attributes.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

namespace Entities {
class Entity {
public:
    Entity(std::unique_ptr<nlohmann::json> clientW, std::unique_ptr<nlohmann::json> clientR);
    virtual void setPosition(int x, int y);
    int getX();
    int getY();
    int getWidth();
    void setWidth(int w);
    int getHeight();
    void setHeight(int w);
    Attributes &getClientW();
    Attributes &getClientR();

protected:
    int _x{}, _y{};
    int _width{};
    int _height{};
    Attributes _clientW{};
    Attributes _clientR{};
};
} // namespace Entities