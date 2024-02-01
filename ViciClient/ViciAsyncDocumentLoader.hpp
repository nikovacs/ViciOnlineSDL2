#pragma once

#include <RmlUi/Core.h>
#include <v8.h>
#include <v8pp/convert.hpp>
#include <v8pp/context.hpp>
#include <string_view>
#include "NetworkAsset.hpp"
#include <unordered_map>
#include <string>
#include <vector>

namespace JS {
	class ViciAsyncDocumentLoader {
	public:
		ViciAsyncDocumentLoader() = delete;
		static v8::Local<v8::Promise> loadDocumentAsync(std::string_view fileName, v8::Local<v8::Context> ctx);
		static v8::Local<v8::Promise> loadDocumentAsyncFromMemory(std::string_view source, std::string_view name, v8::Local<v8::Context> ctx);
		static void attemptResolve();
	private:
		static void _attemptResolveByFileName();
		static void _attemptResolveBySource();
		static void _setIsolateIfNull(v8::Isolate* isolate);
		static v8::Isolate* _isolate;

		// the following are two pairs of parallel lists due to the two ways to load documents in RML
		static std::vector<std::string> _inProgressFileNames;
		static std::vector<std::pair<v8::Global<v8::Promise::Resolver>, v8::Global<v8::Context>>> _inProgressFileNameResolvers;

		static std::vector<std::pair<std::string, std::string>> _inProgressNameSourcePairs;
		static std::vector<std::pair<v8::Global<v8::Promise::Resolver>, v8::Global<v8::Context>>> _inProgressNameSourceResolvers;
	};
}