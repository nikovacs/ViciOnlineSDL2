#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "../ViciEngine/base64.hpp"
#include <fstream>
#include <filesystem>

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
	protected:
		static std::unordered_map<std::string, std::string> _assetIndex;
		static void createFileStructure(std::string_view path);
	};
}