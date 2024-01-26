#include "ViciJSEventListenerHandler.hpp"
#include <v8.h>
#include <v8pp/convert.hpp>
#include <iostream>
#include "RmlEventJSWrapper.hpp"
// include rpcrt4.lib for UuidCreate if windows
#ifdef _WIN32
#pragma comment(lib, "rpcrt4.lib")
#include <windows.h>
#endif


namespace JS {
	void ViciInternalEventListener::ProcessEvent(Rml::Event& event) {
		const Rml::Element* target{ event.GetTargetElement() };
		ViciJSEventListenerHandler::_triggerEvents(target, event);
	}

	ViciInternalEventListener ViciJSEventListenerHandler::_internalEventListener{};
	v8::Global<v8::Symbol> ViciJSEventListenerHandler::_symbol{};

	void ViciJSEventListenerHandler::addEventListener(const Rml::Element* element, const std::string_view eventType, v8::Global<v8::Function> func) {
		v8::Isolate* isolate{ v8::Isolate::GetCurrent() };
		v8::HandleScope handleScope{ isolate };
		v8::Local<v8::Context> context{ isolate->GetCurrentContext() };

		// it should be okay if symbol is empty up until this point, even if other methods are called first
		if (_symbol.IsEmpty()) {
			_symbol.Reset(isolate, v8::Symbol::New(isolate, v8pp::to_v8(isolate, "__VICI__UniqueListenerID")));
		} 
		
		// if the func does not have the symbol, add it with a new uuid
		if (!func.Get(isolate)->Has(context, _symbol.Get(isolate)).FromMaybe(false)) {
			UUID uuid;
			UuidCreate(&uuid);
			char* uuidStr;
			UuidToStringA(&uuid, (RPC_CSTR*)&uuidStr);
			std::cout << "UUID: " << uuidStr << std::endl;
			func.Get(isolate)->Set(context, _symbol.Get(isolate), v8pp::to_v8(isolate, std::string(uuidStr))).Check();
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
			return f.Get(isolate)->Get(context, _symbol.Get(isolate)).ToLocalChecked() == func.Get(isolate)->Get(context, symbol).ToLocalChecked();
			};
		// check if the func does not already exist in the vector
		if (std::find_if(funcs.begin(), funcs.end(), findFunc) == funcs.end()) {
			funcs.emplace_back(isolate, func);
		}
	}

	void ViciJSEventListenerHandler::removeEventListener(const Rml::Element* element, const std::string_view eventType, v8::Global<v8::Function> func) {
		if (!_eventListeners.contains(element)) {
			return;
		}

		std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>>& elementMap{ _eventListeners.at(element) };
		if (!elementMap.contains(eventType.data())) {
			return;
		}

		v8::Isolate* isolate{ v8::Isolate::GetCurrent() };
		v8::HandleScope handleScope{ isolate };
		v8::Local<v8::Context> context{ isolate->GetCurrentContext() };

		std::vector<v8::Global<v8::Function>>& funcs{ elementMap.at(eventType.data()) };
		v8::Local<v8::Symbol> symbol{ _symbol.Get(isolate) };
		auto findFunc = [&func, isolate, context, &symbol](const v8::Global<v8::Function>& f) {
			return f.Get(isolate)->Get(context, _symbol.Get(isolate)).ToLocalChecked() == func.Get(isolate)->Get(context, symbol).ToLocalChecked();
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
	}

	void ViciJSEventListenerHandler::onElementDeleted(const Rml::Element* element) {
		if (!_eventListeners.contains(element)) {
			return;
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

		std::vector<v8::Global<v8::Function>>& funcs{ elementMap.at(ev.GetType()) };
		v8::Isolate* isolate{ v8::Isolate::GetCurrent() };
		v8::HandleScope handleScope{ isolate };
		v8::Local<v8::Context> context{ isolate->GetCurrentContext() };

		v8::Local<v8::Value> args[1]{ v8pp::to_v8(isolate, RmlEventJSWrapper(ev)) };
		for (const v8::Global<v8::Function>& func : funcs) {
			func.Get(isolate)->Call(context, v8::Undefined(isolate), 1, args);
		}
	}
}