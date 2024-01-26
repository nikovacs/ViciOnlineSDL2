#pragma once

#include <RmlUi/Core.h>
#include <v8.h>
#include <unordered_map>
#include <vector>
#include <string_view>
#include <v8pp/context.hpp>

namespace JS {
	class ViciInternalEventListener : public Rml::EventListener {
	public:
		void ProcessEvent(Rml::Event& event) override;
	};

	class ViciJSEventListenerHandler {
	public:
		ViciJSEventListenerHandler() = delete;
		static void addEventListener(Rml::Element* element, std::string_view eventType, v8::Local<v8::Function> func, bool inCapturePhase = false);
		static void removeEventListener(Rml::Element* element, std::string_view eventType, v8::Local<v8::Function> func, bool inCapturePhase = false);
		static void onElementDeleted(Rml::Element* element);
		friend class ViciInternalEventListener;
	private:
		static void _triggerEvents(const Rml::Element* element, Rml::Event& ev);
		static void _updateIsolateIfNull(v8::Isolate* isolate);
		// element -> eventType -> functions
		static std::unordered_map<const Rml::Element*, std::unordered_map<Rml::String, std::vector<v8::Global<v8::Function>>>> _eventListeners;
		static ViciInternalEventListener _internalEventListener;
		static v8::Global<v8::Symbol> _symbol; // used as the key for the v8 functions' unique ids... might have to have one symbol per context of this does not work
		static v8::Isolate* _isolate;
		static std::unordered_map<Rml::Element*, v8::Global<v8::Context>> _elementContextMap;
	};
}