#pragma once

#include <string>
#include <string_view>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include "../ViciEngine/base64.hpp"
#include <fstream>
#include <filesystem>
#include <mutex>

namespace Networking {
	class AssetTransfer {
	public:
		/*
		* Initializes the asset index
		* The asset index is a set of all paths in the asset directory
		* Should be called once at the start of the program
		* Modifications should be done via addAsset and removeAsset
		*/
		static void initializeIndex();

		/*
		* Adds the given file to the asset index
		*/
		static void addAssetToIndex(std::string_view filePath);

		/*
		* Checks whether the asset index contains the given file path
		* 
		* @param filePath: The file path to check for
		* @return True if the file path is in the asset index, false otherwise
		*/
		static bool indexContains(std::string_view filePath);

		/*
		* Removes the given file from the asset index
		*/
		static void removeAssetFromIndex(std::string_view filePath);
		
		/*
		* Returns the file data of the file at the given path
		* If the file does not exist, an empty string is returned
		* 
		* @param path: The path to the file (with filename and extension)
		* @return The file data
		*/
		static inline std::string readFile(std::string_view path) {
			std::string fullPath {"Assets/" + std::string(path)};
			std::ifstream file(fullPath, std::ios::binary);

			if (!file) {
				file.close();
				return "";
			}

			std::string fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();

			return fileData;
		}

		/*
		* Writes the given file data to the file at the given path
		* If the file does not exist, it is created
		* If the file does exist, it is overwritten
		* 
		* @param path: The path to the file (with filename and extension)
		* @param fileData: The data to write to the file
		* @return True if the file was written successfully, false otherwise
		*/
		static inline bool writeFile(std::string_view path, std::string_view fileData) {
			std::string fullPath{ "Assets/" + std::string(path) };
			createFileStructure(fullPath);
			std::ofstream file(fullPath, std::ios::binary);

			if (!file) {
				file.close();
				return false;
			}

			file.write(fileData.data(), fileData.length());
			file.close();

			return true;
		}
	protected:
		/*
		* The asset index
		* A set of all paths in the asset directory
		*/
		static std::unordered_set<std::string> _assetIndex;

		/*
		* The mutex for the asset index
		* since it could be modified by multiple threads
		*/
		static std::mutex _indexMutex;

		/*
		* Creates the file structure for the given path if it does not already exist
		*/
		static void createFileStructure(std::string_view path);
	};
}