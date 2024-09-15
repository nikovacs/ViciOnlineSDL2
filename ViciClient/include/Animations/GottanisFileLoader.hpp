#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <gottanis/include/IFileLoader.h>
#include <nlohmann/json.hpp>
#include "ViciClient/include/Networking/NetworkAsset.hpp"

namespace Animations {
class GottanisFileLoader : public gottanis::IFileLoader {
public:
    GottanisFileLoader() = default;
    ~GottanisFileLoader() override = default;
    
    nlohmann::json *loadJson(std::string_view fileName) override;
    void update();
private:
    // map file name to asset
    std::unordered_map<std::string, std::unique_ptr<Networking::NetworkAsset<nlohmann::json>>> _jsonAssets{};
    std::unordered_set<std::string> _pendingFiles{};
};
} // namespace Animations