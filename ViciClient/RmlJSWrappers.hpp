#pragma once

#include <RmlUi/Core.h>
#include <string>
#include <vector>

namespace JS {
	class RmlElementJSWrapper;
	class RmlDocumentJSWrapper;

	class RmlContextJSWrapper {
	public:
		RmlContextJSWrapper(Rml::Context& context);
		virtual ~RmlContextJSWrapper();
		void setDimensions(std::vector<int> dimensions);
		std::vector<int> getDimensions();
		void setDensityIndependentPixelRatio(float ratio);
		float getDensityIndependentPixelRatio();
		RmlDocumentJSWrapper createDocument(std::string instancerName = "body");
		//RmlDocumentJSWrapper loadDocument(std::string documentPath); Can't have this since the file may not be local. Could potentially make it async and return a promise
		//RmlDocumentJSWrapper loadDocument(Stream docStream); Not Relevant
		RmlDocumentJSWrapper loadDocumentFromString(std::string document, std::string docName);
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
	};

	class RmlDocumentJSWrapper {
	public:
		RmlDocumentJSWrapper(Rml::ElementDocument* document);
		virtual ~RmlDocumentJSWrapper();
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
		bool isValidDocument();
		Rml::ElementDocument* getUnderlyingDocument();
	private:
		Rml::ElementDocument* _document;
	};

	class RmlElementJSWrapper {
	public:
		RmlElementJSWrapper(Rml::Element* element);
		virtual ~RmlElementJSWrapper();
		void setClass(std::string className, bool activate);
		bool isClassSet(std::string className);
		void setClassNames(std::string classNames);
		std::string getClassNames();
		//RmlStyleSheetJSWrapper getStyleSheet();
		std::string getAddress(bool includePseudoClasses = false, bool includeParents = true);
		void setOffset(float x, float y, RmlElementJSWrapper offsetParent, bool offsetFixed = false);
		//std::vector<int> getRelativeOffset(std::string area = "CONTENT");
		//std::vector<int> getAbsoluteOffset(std::string area = "CONTENT");
		//void setClientArea(std::string clientArea);
		//std::string getClientArea();
		//void setBox();
		//void GetBox();
		//void getBox(int index, int xOffset, int yOffset);
		int getNumBoxes();
		float getBaseline();
		//bool getInstrinsicDimensions();
		bool isPointWithinElement(float x, float y);
		bool isVisible(bool includeAncestors = false);
		float getZIndex();
		//RmlFontFaceHandleJSWrapper getFontFaceHandle();
		bool setProperty(std::string name, std::string value);
		//bool setProperty(PropertyId id, const Property& property) ??
		void removeProperty(std::string name);
		//RmlPropertyJSWrapper getProperty(std::string name);
		//RmlPropertyJSWrapper getProperty(RmlPropertyIdJSWrapper id);
		//RmlPropertyJSWrapper getLocalProperty(std::string name);
		//RmlPropertyJSWrapper getLocalProperty(RmlPropertyIdJSWrapper id);
		//float resolveNumericProperty(RmlPropertyJSWrapper property, float baseValue);
		float resolveNumericProperty(std::string property);
		std::vector<float> getContainingBlock();
		//RmlPositionJSWrapper getPosition();
		//RmlFloatJSWrapper getFloat();
		//RmlDisplayJSWrapper getDisplay();
		float getLineHeight();
		bool project(float x, float y);
		//bool animate
		//bool addAnimationKey
		//IterateLocalProperties
		void setPseudoClass(std::string pseudoClass, bool activate);
		bool isPseudoClassSet(std::string pseudoClass);
		bool arePseudoClassesSet(std::vector<std::string> pseudoClasses);
		std::vector<std::string> getActivePseudoClasses();
		// not even half way, come back to this when we need it

		// for internal use only
		bool isValidElement();
		Rml::Element* getUnderlyingElement();
	private:
		Rml::Element* _element;
	};
}