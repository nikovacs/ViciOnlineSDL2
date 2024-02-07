#include "ViciAsyncDocumentLoader.hpp"
#include "RmlDocumentJSWrapper.hpp"
#include <v8pp/class.hpp>

namespace JS {
	std::vector<std::string> ViciAsyncDocumentLoader::_inProgressFileNames{};
	std::vector<std::pair<v8::Global<v8::Promise::Resolver>, v8::Global<v8::Context>>> ViciAsyncDocumentLoader::_inProgressFileNameResolvers{};
	std::vector<std::pair<std::string, std::string>> ViciAsyncDocumentLoader::_inProgressNameSourcePairs{};
	std::vector<std::pair<v8::Global<v8::Promise::Resolver>, v8::Global<v8::Context>>> ViciAsyncDocumentLoader::_inProgressNameSourceResolvers{};
	v8::Isolate* ViciAsyncDocumentLoader::_isolate{ nullptr };

	v8::Local<v8::Promise> ViciAsyncDocumentLoader::loadDocumentAsync(std::string_view fileName, v8::Local<v8::Context> ctx) {
		_setIsolateIfNull(ctx->GetIsolate());
		v8::Local<v8::Promise::Resolver> resolver{ v8::Promise::Resolver::New(ctx).ToLocalChecked() };
		v8::Local<v8::Promise> promise{ resolver->GetPromise() };

		_inProgressFileNames.push_back(fileName.data());
		_inProgressFileNameResolvers.push_back({ v8::Global<v8::Promise::Resolver>{isolate, resolver}, v8::Global<v8::Context>{isolate, ctx} });

		return promise;
	}

	v8::Local<v8::Promise> ViciAsyncDocumentLoader::loadDocumentAsyncFromMemory(std::string_view source, std::string_view name, v8::Local<v8::Context> ctx) {
		_setIsolateIfNull(ctx->GetIsolate());
		v8::Local<v8::Promise::Resolver> resolver{ v8::Promise::Resolver::New(ctx).ToLocalChecked() };
		v8::Local<v8::Promise> promise{ resolver->GetPromise() };
		_inProgressNameSourcePairs.push_back({ name.data(), source.data() });
		_inProgressNameSourceResolvers.push_back({ v8::Global<v8::Promise::Resolver>{isolate, resolver}, v8::Global<v8::Context>{isolate, ctx} });

		return promise;
	}

	void ViciAsyncDocumentLoader::attemptResolve() {
		_attemptResolveByFileName();
		_attemptResolveBySource();
	}

	void ViciAsyncDocumentLoader::_attemptResolveByFileName() {
		std::vector<size_t> toRemove{};
		for (size_t i{ 0 }; i < _inProgressFileNames.size(); ++i) {
			v8::HandleScope handleScope{ _isolate };
			v8::Local<v8::Context> ctx{ _inProgressFileNameResolvers[i].second.Get(_isolate) };
			v8::Context::Scope contextScope{ ctx };
			v8::Local<v8::Promise::Resolver> resolver{ _inProgressFileNameResolvers[i].first.Get(_isolate) };

			const std::string& fileName{ _inProgressFileNames[i] };

			try {
				Rml::ElementDocument* doc{ Rml::GetContext("GameScene")->LoadDocument(fileName)};
				_isolate->Enter();
				if (doc) {
					resolver->Resolve(ctx, v8pp::to_v8(_isolate, RmlDocumentJSWrapper(doc)));
				}
				else {
					resolver->Reject(ctx, v8pp::to_v8(_isolate, "Failed to load document"));
				}
				_isolate->Exit();
				toRemove.push_back(i);
			}
			catch (const std::string& e) {
				Networking::NetworkAsset<std::string> asset{ e };
				// even though this pops off the stack immediately,
				// the asset will still be requested and placed in the cache
			}
		}

		for (int i{ static_cast<int>(toRemove.size()) - 1 }; i >= 0; --i) {
			_inProgressFileNames.erase(_inProgressFileNames.begin() + toRemove[i]);
			_inProgressFileNameResolvers.erase(_inProgressFileNameResolvers.begin() + toRemove[i]);
		}
	}

	void ViciAsyncDocumentLoader::_attemptResolveBySource() {
		std::vector<size_t> toRemove{};
		for (size_t i{ 0 }; i < _inProgressNameSourcePairs.size(); ++i) {
			v8::HandleScope handleScope{ _isolate };
			v8::Local<v8::Context> ctx{ _inProgressNameSourceResolvers[i].second.Get(_isolate) };
			v8::Context::Scope contextScope{ ctx };
			v8::Local<v8::Promise::Resolver> resolver{ _inProgressNameSourceResolvers[i].first.Get(_isolate) };

			const std::string& name{ _inProgressNameSourcePairs[i].first };
			const std::string& source{ _inProgressNameSourcePairs[i].second };

			try {
				Rml::ElementDocument* doc{ Rml::GetContext("GameScene")->LoadDocumentFromMemory(source, name) };
				if (doc) {
					resolver->Resolve(ctx, v8pp::to_v8(_isolate, RmlDocumentJSWrapper(doc)));
				}
				else {
					resolver->Reject(ctx, v8pp::to_v8(_isolate, "Failed to load document"));
				}
				toRemove.push_back(i);
			}
			catch (const std::string& e) {
				Networking::NetworkAsset<std::string> asset{ e };
				// even though this pops off the stack immediately,
				// the asset will still be requested and placed in the cache
			}
		}

		for (int i{ static_cast<int>(toRemove.size()) - 1 }; i >= 0; --i) {
			_inProgressNameSourcePairs.erase(_inProgressNameSourcePairs.begin() + toRemove[i]);
			_inProgressNameSourceResolvers.erase(_inProgressNameSourceResolvers.begin() + toRemove[i]);
		}
	}

	void ViciAsyncDocumentLoader::_setIsolateIfNull(v8::Isolate* isolate) {
		if (!_isolate) {
			_isolate = isolate;
		}
	}
}