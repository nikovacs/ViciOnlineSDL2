#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include "ViciCore/include/Utility/base64.hpp"

namespace Networking {
	class AssetTransfer {
	public:
		static void initializeIndex();
		
		static inline std::string readFile(std::string_view path) {
			createFileStructure(path);
			std::ifstream file(path.data(), std::ios::binary);

			if (!file) {
				file.close();
				return "";
			}

			std::string fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();

			return fileData;
		}
		static inline bool writeFile(std::string_view path, std::string_view file_data) {
			createFileStructure(path);
			std::ofstream file(path.data(), std::ios::binary);

			if (!file) {
				file.close();
				return false;
			}

			file.write(file_data.data(), file_data.length());
			file.close();

			return true;
		}

		static bool fileExists(std::string_view fileName);

	protected:
		static std::unordered_map<std::string, std::string> _assetIndex;
		static void createFileStructure(std::string_view path);
		static std::string toLower(const std::string& str);
		static std::string normalizePath(const std::string& path);
		static void addScriptToIndex(const std::filesystem::path& path);
	};
}