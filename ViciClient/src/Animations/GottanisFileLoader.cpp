#include "ViciClient/include/Animations/GottanisFileLoader.hpp"
#include <iostream>

namespace Animations {
nlohmann::json *GottanisFileLoader::loadJson(std::string_view fileName) {
    std::cout << "Gottanis requesting JSON file: " << fileName << std::endl;
    nlohmann::json *asset = nullptr;
    if (_jsonAssets.contains(fileName.data())) {
        auto networkAsset = _jsonAssets[fileName.data()].get();
        asset = networkAsset->getValue();
    } else {
        _jsonAssets[fileName.data()] = std::make_unique<Networking::NetworkAsset<nlohmann::json>>(fileName.data());
        asset = _jsonAssets[fileName.data()]->getValue();
    }

    if (!asset) {
        _pendingFiles.insert(fileName.data());
    }

    return asset;
}

void GottanisFileLoader::update() {
    for (const auto &fileName : _pendingFiles) {
        auto asset = _jsonAssets[fileName]->getValue();
        if (asset) {
            _pendingFiles.erase(fileName);
        }
        // tell gottanis to load the file
    }
}
} // namespace Animations