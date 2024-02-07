#pragma once

#include <RmlUi/Core.h>
#include <string>
#include <vector>
#include "RmlDocumentJSWrapper.hpp"

namespace JS {
	class RmlContextJSWrapper {
	public:
		RmlContextJSWrapper(Rml::Context& context, v8::Local<v8::Context> v8Ctx);
		virtual ~RmlContextJSWrapper();
		void setDimensions(std::vector<int> dimensions);
		std::vector<int> getDimensions();
		void setDensityIndependentPixelRatio(float ratio);
		float getDensityIndependentPixelRatio();
		RmlDocumentJSWrapper createDocument(std::string instancerName = "body");
		v8::Local<v8::Promise> loadDocument(std::string documentPath);
		v8::Local<v8::Promise> loadDocumentFromString(std::string document, std::string docName);
		void unloadDocument(RmlDocumentJSWrapper doc);
		void unloadAllDocuments();
		void enableMouseCursor(bool enable);
		void activateTheme(std::string, bool activate);
		bool isThemeActive(std::string);
		RmlDocumentJSWrapper getDocument(std::string docId);
		RmlElementJSWrapper getHoverElement();
		RmlElementJSWrapper getFocusElement();
		RmlElementJSWrapper getRootElement();
		RmlElementJSWrapper getElementAtPoint(std::vector<float> point, RmlElementJSWrapper ignoreElement, RmlElementJSWrapper element);
		void pullDocumentToFront(RmlDocumentJSWrapper document);
		void pushDocumentToBack(RmlDocumentJSWrapper document);
		void unfocusDocument(RmlDocumentJSWrapper document);
		//void addEventListener();
		//void removeEventListener();
		//bool processKeyDown
		//bool processKeyUp
		bool processTextInput(std::string input);
		bool processMouseMove(int x, int y, int keyModifierState);
		//bool processMouseButtonDown(int buttonIndex, int keyModifierState);
		//bool processMouseButtonUp(int buttonIndex, int keyModifierState);
		//bool processMouseWheel(int wheelDelta, int keyModifierState);
		//bool processMouseWheel(float xDelta, float yDelta, int keyModifierState);
		bool processMouseLeave();
		bool isMouseInteracting();
		//void setDefaultScrollBehavior
		//RenderInterface* getRenderInterface();
		bool getActiveClipRegion(std::vector<int> origin, std::vector<int> dimensions);
		void setActiveClipRegion(std::vector<int> origin, std::vector<int> dimensions);
		//void setInstancer // definitely don't need this
		//DataModelConstructor createDataModel
		//DataModelConstructor getDataModel
		bool removeDataModel(std::string modelName);
		void setDocumentsBaseTag(std::string baseTag);
		std::string getDocumentsBaseTag();
		//void requestNextUpdate(double delay);
		//double getNextUpdateDelay();

		// for internal use only
		Rml::Context& getUnderlyingContext();
	private:
		Rml::Context& _ctx;
		v8::Isolate* _isolate;
	};
}