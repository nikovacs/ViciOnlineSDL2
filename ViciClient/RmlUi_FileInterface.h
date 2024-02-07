#pragma once

#include <RmlUi/Core.h>
#include <unordered_map>
#include "NetworkAsset.hpp"
#include <memory>

namespace Rml {
	class RmlUi_FileInterface : public Rml::FileInterface {
	public:
		RmlUi_FileInterface() = default;
		virtual ~RmlUi_FileInterface();
		FileHandle Open(const String& path) override;
		void Close(FileHandle file) override;
		size_t Read(void* buffer, size_t size, FileHandle file) override;
		bool Seek(FileHandle file, long offset, int origin) override;
		size_t Tell(FileHandle file) override;
		bool LoadFile(const String& path, String& out_data) override;
	private:
		std::unordered_map<FileHandle, size_t> _positions{};
		std::unordered_map<String, int> _refCounts{};
		std::unordered_map<String, std::unique_ptr<Networking::NetworkAsset<String>>> _assets{};
		std::unordered_map<FileHandle, String> _fileHandles{};
	};
}