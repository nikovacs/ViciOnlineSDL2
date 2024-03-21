#include "AssetTransfer.hpp"
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

std::unordered_set<std::string> Networking::AssetTransfer::_assetIndex{};
std::mutex Networking::AssetTransfer::_indexMutex{};

void Networking::AssetTransfer::initializeIndex() {
	std::lock_guard<std::mutex> lock(_indexMutex);
	if (!fs::exists("Assets"))
		return;
	for (const auto& entry : fs::recursive_directory_iterator("Assets")) {
		const auto& path = entry.path();
		if (fs::is_regular_file(path)) {
			_assetIndex.insert(path.lexically_relative("Assets").generic_string());
		}
	}
}

void Networking::AssetTransfer::addAssetToIndex(std::string_view path) {
	std::lock_guard<std::mutex> lock(_indexMutex);
	_assetIndex.insert(path.data());
}

void Networking::AssetTransfer::removeAssetFromIndex(std::string_view path) {
	std::lock_guard<std::mutex> lock(_indexMutex);
	_assetIndex.erase(path.data());
}

bool Networking::AssetTransfer::indexContains(std::string_view path) {
	std::lock_guard<std::mutex> lock(_indexMutex);
	return _assetIndex.contains(path.data());
}

void Networking::AssetTransfer::createFileStructure(std::string_view path) {
	fs::path dirPath{ path };
	dirPath.remove_filename();
	fs::create_directories(dirPath);
}