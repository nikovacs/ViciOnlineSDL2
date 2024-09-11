#include "ViciClient/include/GUI/RmlUi_FileInterface.h"
#include "ViciClient/include/Networking/NetworkAsset.hpp"

namespace Rml {
	RmlUi_FileInterface::~RmlUi_FileInterface() {
	
	}

	FileHandle RmlUi_FileInterface::Open(const String& file_name)
	{
		if (!_assets.contains(file_name)) {
			_assets.emplace(file_name, std::make_unique<Networking::NetworkAsset<String>>(file_name));
		}

		String* asset{ _assets[file_name]->getValue() };
		if (asset == nullptr) {
			throw file_name;
		}

		FileHandle file_handle{ reinterpret_cast<FileHandle>(asset) };
		_fileHandles.emplace(file_handle, file_name);

		if (!_refCounts.contains(file_name)) {
			_refCounts.emplace(file_name, 1);
		}
		else {
			_refCounts[file_name]++;
		}

		_positions.emplace(file_handle, 0);

		return file_handle;
	}

	void RmlUi_FileInterface::Close(FileHandle file)
	{
		String& file_name{ _fileHandles[file] };
		_refCounts[file_name]--;
		if (_refCounts[file_name] == 0) {
			_assets.erase(file_name);
			_refCounts.erase(file_name);
		}
		_positions.erase(file);
		_fileHandles.erase(file);
	}

	size_t RmlUi_FileInterface::Read(void* buffer, size_t size, FileHandle file)
	{
		String& str = *reinterpret_cast<String*>(file);
		size_t& position = _positions[file];

		// Calculate the remaining bytes to read from the current position
		size_t bytes_remaining = str.size() - position;

		// Determine how many bytes to read (minimum of bytes remaining and requested size)
		size_t bytes_to_copy = (std::min)(bytes_remaining, size);

		// Copy bytes from the string to the buffer
		memcpy(buffer, str.data() + position, bytes_to_copy);

		// Update the position
		position += bytes_to_copy;

		return bytes_to_copy;
	}

	bool RmlUi_FileInterface::Seek(FileHandle file, long offset, int origin)
	{
		String& str = *reinterpret_cast<String*>(file);
		size_t& position = _positions[file];
		switch (origin) {
		case SEEK_SET:
			if (offset >= 0 && offset < static_cast<long>(str.size())) {
				position = offset;
				return true;
			}
			break;
		case SEEK_CUR:
			if (static_cast<long>(position) + offset >= 0 && static_cast<long>(position) + offset < static_cast<long>(str.size())) {
				position += offset;
				return true;
			}
			break;
		case SEEK_END:
			if (offset <= 0 && static_cast<long>(str.size()) + offset >= 0) {
				position = str.size() + offset;
				return true;
			}
			break;
		}
		return false;
	}

	size_t RmlUi_FileInterface::Tell(FileHandle file)
	{
		return _positions[file];
	}

	bool RmlUi_FileInterface::LoadFile(const String& file_name, String& out_data) {
		// entire file is loaded into a separate buffer, so we don't need to bother tracking it
		Networking::NetworkAsset<String> asset{ file_name };
		if (asset.getValue() == nullptr) {
			throw file_name;
		}
		else {
			out_data = *asset.getValue();
		}
		return true;
	}
}