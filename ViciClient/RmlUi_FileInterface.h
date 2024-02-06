#pragma once

#include <RmlUi/Core.h>

namespace Rml {
	class RmlUi_FileInterface : public Rml::FileInterface {
	public:
		virtual ~RmlUi_FileInterface();
		FileHandle Open(const String& path) override;
		void Close(FileHandle file) override;
		size_t Read(void* buffer, size_t size, FileHandle file) override;
		bool Seek(FileHandle file, long offset, int origin) override;
		size_t Tell(FileHandle file) override;
		bool LoadFile(const String& path, String& out_data) override;
	};
}