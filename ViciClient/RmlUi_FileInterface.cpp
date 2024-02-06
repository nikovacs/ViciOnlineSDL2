#include "RmlUi_FileInterface.h"
#include "NetworkAsset.hpp"

namespace Rml {
	RmlUi_FileInterface::~RmlUi_FileInterface() {}

	// same as FileInterfaceDefault from RmlUi vvv
	FileHandle RmlUi_FileInterface::Open(const String& path)
	{
		return (FileHandle)fopen(path.c_str(), "rb");
	}

	void RmlUi_FileInterface::Close(FileHandle file)
	{
		fclose((FILE*)file);
	}

	size_t RmlUi_FileInterface::Read(void* buffer, size_t size, FileHandle file)
	{
		return fread(buffer, 1, size, (FILE*)file);
	}

	bool RmlUi_FileInterface::Seek(FileHandle file, long offset, int origin)
	{
		return fseek((FILE*)file, offset, origin) == 0;
	}

	size_t RmlUi_FileInterface::Tell(FileHandle file)
	{
		return ftell((FILE*)file);
	}
	// same as FileInterfaceDefault from RmlUi ^^^

	// custom LoadFile that will throw an exception if the file is not found
	bool RmlUi_FileInterface::LoadFile(const String& file_name, String& out_data) {
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