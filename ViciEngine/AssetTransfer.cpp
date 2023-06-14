#include "AssetTransfer.hpp"
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

std::unordered_map<std::string, std::string> Networking::AssetTransfer::_assetIndex = std::unordered_map<std::string, std::string>();

void Networking::AssetTransfer::initializeIndex() {
	//createFileStructure("Assets/ViciDefaults");
	if (!fs::exists("Assets"))
		return;
	for (const auto& entry : fs::recursive_directory_iterator("Assets")) {
		const auto& path = entry.path();
		if (fs::is_regular_file(path)) {
			auto filename_str = path.filename().string();
			std::transform(filename_str.begin(), filename_str.end(), filename_str.begin(),
				[](unsigned char c) { return std::tolower(c); });
			_assetIndex.insert(std::make_pair(filename_str, path.string()));
		}
	}
}

void Networking::AssetTransfer::createFileStructure(std::string_view path) {
	std::cout << path << std::endl;
	fs::path dirPath{ path };
	dirPath.remove_filename();
	std::cout << dirPath << std::endl;
	fs::create_directories(dirPath);
}