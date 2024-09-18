#include "header_wrappers/v8_wrapper.h"
#include "header_wrappers/v8pp_wrapper.h"
#include <iostream>
#include "ViciClient/include/JSRuntime/RmlUI/ViciJSEventListenerHandler.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/RmlEventJSWrapper.hpp"

// include rpcrt4.lib for UuidCreate if windows
#ifdef _WIN32
#pragma comment(lib, "rpcrt4.lib")
#include "header_wrappers/windows_wrapper.h"
#endif

#ifdef __APPLE__
#include <uuid/uuid.h>
#endif



namespace JS {
	void ViciInternalEventListener::ProcessEvent(Rml::Event& event) {
		const Rml::Element* target{ event.GetTargetElement() };
		ViciJSEventListenerHandler::_triggerEvents(target, event);
	}

	std::unordered_map<const Rml::Element*, std::unordered_map<Rml::String, std::vector<v8::Global<v8::Function>>>> ViciJSEventListenerHandler::_eventListeners{};
	ViciInternalEventListener ViciJSEventListenerHandler::_internalEventListener{};
	v8::Global<v8::Symbol> ViciJSEventListenerHandler::_symbol{};
	v8::Isolate* ViciJSEventListenerHandler::_isolate{ nullptr };
	std::unordered_map<Rml::Element*, v8::Global<v8::Context>> ViciJSEventListenerHandler::_elementContextMap{};

	void ViciJSEventListenerHandler::deinitialize() {
		_symbol.Reset();
	}

	void ViciJSEventListenerHandler::addEventListener(Rml::Element* element, std::string_view eventType, v8::Local<v8::Function> func, bool inCapturePhase) {
		v8::Isolate* isolate{ v8::Isolate::GetCurrent() };
		_updateIsolateIfNull(isolate);
		v8::HandleScope handleScope{ isolate };
		v8::Local<v8::Context> context{ isolate->GetCurrentContext() };
		if (!_elementContextMap.contains(element)) {
			_elementContextMap.emplace(element, v8::Global<v8::Context>{isolate, context});
		}

		// it should be okay if symbol is empty up until this point, even if other methods are called first
		if (_symbol.IsEmpty()) {
			_symbol.Reset(isolate, v8::Symbol::New(isolate, v8pp::to_v8(isolate, "__VICI__UniqueListenerID")));
		} 
		
		// if the func does not have the symbol, add it with a new uuid
		if (!func->Has(context, _symbol.Get(isolate)).FromMaybe(false)) {
#ifdef _WIN32
			UUID uuid;
			UuidCreate(&uuid);
			char* uuidStr;
			UuidToStringA(&uuid, (RPC_CSTR*)&uuidStr);
			func->Set(context, _symbol.Get(isolate), v8pp::to_v8(isolate, std::string(uuidStr))).Check();
			// TODO is uuidStr a memory leak?
#elif __APPLE__
			uuid_t uuid;
			uuid_generate(uuid);
			char uuidStr[37];
			uuid_unparse(uuid, uuidStr);
			func->Set(context, _symbol.Get(isolate), v8pp::to_v8(isolate, std::string(uuidStr))).Check();
#else
			throw std::runtime_error("Not implemented for this platform");
#endif
		}

		if (!_eventListeners.contains(element)) {
			_eventListeners.emplace(element, std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>>{});
		}

		std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>>& elementMap{ _eventListeners.at(element) };
		if (!elementMap.contains(eventType.data())) {
			elementMap.emplace(eventType.data(), std::vector<v8::Global<v8::Function>>{});
		}

		std::vector<v8::Global<v8::Function>>& funcs{ elementMap.at(eventType.data()) };
		v8::Local<v8::Symbol> symbol{ _symbol.Get(isolate) };
		auto findFunc = [&func, isolate, context, &symbol](const v8::Global<v8::Function>& f) {
			return f.Get(isolate)->Get(context, _symbol.Get(isolate)).ToLocalChecked() == func->Get(context, symbol).ToLocalChecked();
			};
		// check if the func does not already exist in the vector
		if (std::find_if(funcs.begin(), funcs.end(), findFunc) == funcs.end()) {
			funcs.emplace_back(isolate, func);
		}

		element->AddEventListener(eventType.data(), &_internalEventListener, inCapturePhase);
	}

	void ViciJSEventListenerHandler::removeEventListener(Rml::Element* element, std::string_view eventType, v8::Local<v8::Function> func, bool inCapturePhase) {
		if (!_eventListeners.contains(element)) {
			return;
		}

		std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>>& elementMap{ _eventListeners.at(element) };
		if (!elementMap.contains(eventType.data())) {
			return;
		}

		v8::Isolate* isolate{ v8::Isolate::GetCurrent() };
		_updateIsolateIfNull(isolate);
		v8::HandleScope handleScope{ isolate };
		v8::Local<v8::Context> context{ isolate->GetCurrentContext() };

		std::vector<v8::Global<v8::Function>>& funcs{ elementMap.at(eventType.data()) };
		v8::Local<v8::Symbol> symbol{ _symbol.Get(isolate) };
		auto findFunc = [&func, isolate, context, &symbol](const v8::Global<v8::Function>& f) {
			return f.Get(isolate)->Get(context, symbol).ToLocalChecked() == func->Get(context, symbol).ToLocalChecked();
		};
		auto funcIt{ std::find_if(funcs.begin(), funcs.end(), findFunc) };
		if (funcIt != funcs.end()) {
			funcs.erase(funcIt);
		}

		if (funcs.empty()) {
			elementMap.erase(eventType.data());
		}

		if (elementMap.empty()) {
			_eventListeners.erase(element);
		}

		element->RemoveEventListener(eventType.data(), &_internalEventListener, inCapturePhase);
	}

	void ViciJSEventListenerHandler::onElementDeleted(Rml::Element* element) {
		if (_elementContextMap.contains(element)) {
			_elementContextMap.erase(element);
		}

		if (!_eventListeners.contains(element)) {
			return;
		}
		auto& map = _eventListeners.at(element);
		// get a list of all the keys in the map
		std::vector<Rml::String> keys;
		for (auto& [key, value] : map) {
			element->RemoveEventListener(key, &_internalEventListener, false);
			element->RemoveEventListener(key, &_internalEventListener, true);
		}
		_eventListeners.erase(element);
	}

	void ViciJSEventListenerHandler::_triggerEvents(const Rml::Element* element, Rml::Event& ev) {
		if (!_eventListeners.contains(element)) {
			return;
		}

		std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>>& elementMap{ _eventListeners.at(element) };
		if (!elementMap.contains(ev.GetType())) {
			return;
		}

		v8::Global<v8::Context>& context{ _elementContextMap.at(const_cast<Rml::Element*>(element)) };
		v8::HandleScope scope{ _isolate };
		v8::Local<v8::Context> localContext{context.Get(_isolate)};
		// this function call does not originate from inside the isolate, so we need to enter it manually
		_isolate->Enter();

		v8::Local<v8::Value> args[1];
		args[0] = v8pp::to_v8(_isolate, RmlEventJSWrapper(ev));
		for (const v8::Global<v8::Function>& func : elementMap.at(ev.GetType())) {
			(void)func.Get(_isolate)->Call(localContext, v8::Undefined(_isolate), 1, args);
		}

		_isolate->Exit();
	}

	void ViciJSEventListenerHandler::_updateIsolateIfNull(v8::Isolate* isolate) {
		if (_isolate == nullptr) {
			_isolate = isolate;
		}
	}
}