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

			return base64::to_base64(fileData);
		}
		static inline bool writeFile(std::string_view path, std::string_view file_data_b64) {
			createFileStructure(path);
			std::ofstream file(path.data(), std::ios::binary);

			if (!file) {
				file.close();
				return false;
			}

			std::string file_data{ base64::from_base64(file_data_b64.data()) };

			file.write(file_data.data(), file_data.length());
			file.close();

			return true;
		}
	protected:
		static std::unordered_map<std::string, std::string> _assetIndex;
		static void createFileStructure(std::string_view path);
	};
}