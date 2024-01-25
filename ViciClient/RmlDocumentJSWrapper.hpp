#pragma once

#include <RmlUi/Core.h>
#include <string>
#include "RmlElementJSWrapper.hpp"

namespace JS {
	class RmlDocumentJSWrapper : public RmlElementJSWrapper {
	public:
		RmlDocumentJSWrapper(Rml::ElementDocument* document);
		virtual ~RmlDocumentJSWrapper();
		//void processHeader
		void setTitle(std::string title);
		std::string getTitle();
		std::string getSourceURL();
		//RmlStyleSheetJSWrapper getStyleSheet;
		void reloadStyleSheet();
		//RmlStyleSheetContainerJSWrapper getSTyleSheetContainer();
		//void SetStyleSheetContainer(RmlStyleSheetContainerJSWrapper styleSheet);
		void pullToFront();
		void pushToBack();
		//void show(std::string modalFlag = "None", std::string focusFlag = "Auto");
		void hide();
		void close();
		//RmlElementPtrJSWrapper createElement(std::string name);
		//RmlElementPtrJSWrapper createTextNode(std::string text);
		bool isModal();
		//void loadInlineScript(std::string content, std::string sourcePath, int sourceLine);
		//void loadExternalScript(std::string path); do we even need this?

		// for internal use only
		Rml::ElementDocument* getUnderlyingDocument();
	private:
		Rml::ElementDocument* _document;
	};
}