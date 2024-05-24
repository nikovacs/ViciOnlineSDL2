#include "AssetTransfer.hpp"
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

std::unordered_map<std::string, std::string> Networking::AssetTransfer::_assetIndex = std::unordered_map<std::string, std::string>();

void Networking::AssetTransfer::initializeIndex() {
	if (!fs::exists("assets"))
		return;
	for (const auto& entry : fs::recursive_directory_iterator("assets")) {
		const std::filesystem::path& path = entry.path();
		std::filesystem::path normalizedPath = normalizePath(path.string());

		// special handling for scripts
		if (fs::is_regular_file(normalizedPath)) {
			if (normalizedPath.filename().string()[0] == '.') {
				continue;
			}

			if (toLower(normalizedPath.string()).find("/scripts") != std::string::npos) {
				addScriptToIndex(normalizedPath);
				continue;
			}

			std::string filename_str = toLower(normalizedPath.filename().string());
			_assetIndex.insert(std::make_pair(filename_str, normalizedPath.string()));
		}
	}
}

bool Networking::AssetTransfer::fileExists(std::string_view fileName) {
	return _assetIndex.contains(toLower(fileName.data()));
}

void Networking::AssetTransfer::addScriptToIndex(const std::filesystem::path& path) {
	std::string filename_str = toLower(path.filename().string());

	if (toLower(path.string()).find("scripts/clientside") != std::string::npos) {
		filename_str = "cs/" + filename_str;
	}
	else if (toLower(path.string()).find("scripts/serverside") != std::string::npos) {
		filename_str = "ss/" + filename_str;
	}
	else if (toLower(path.string()).find("scripts/shared") != std::string::npos) {
		filename_str = "sh/" + filename_str;
	}

	std::cout << "Adding script to index: " << filename_str << std::endl;
	_assetIndex.insert(std::make_pair(filename_str, path.string()));
}

void Networking::AssetTransfer::createFileStructure(std::string_view path) {
	fs::path dirPath{ path };
	dirPath.remove_filename();
	fs::create_directories(dirPath);
}

std::string Networking::AssetTransfer::toLower(const std::string& str) {
	std::string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
	return lowerStr;
}

std::string Networking::AssetTransfer::normalizePath(const std::string& path) {
	std::string normalizedPath = path;
	std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
	return normalizedPath;
}