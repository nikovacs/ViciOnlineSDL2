#pragma once

#include <RmlUi/Core.h>
#include <string>
#include <unordered_map>
#include <v8.h>

namespace JS {
	class RmlDocumentJSWrapper; // get around circular include
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
		void setOffset(std::vector<float> offset, RmlElementJSWrapper offsetParent, bool offsetFixed = false);
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
		//float resolveNumericValue(std::string property);
		std::vector<float> getContainingBlock();
		//RmlPositionJSWrapper getPosition();
		//RmlFloatJSWrapper getFloat();
		//RmlDisplayJSWrapper getDisplay();
		float getLineHeight();
		bool project(std::vector<float> point);
		//bool animate
		//bool addAnimationKey
		//IterateLocalProperties
		void setPseudoClass(std::string pseudoClass, bool activate);
		bool isPseudoClassSet(std::string pseudoClass);
		bool arePseudoClassesSet(std::vector<std::string> pseudoClasses);
		std::vector<std::string> getActivePseudoClasses();
		void setAttribute(std::string name, std::string value); // do we need other overloads?
		//std::string getAttribute(std::string name); // all 3 overloads
		bool hasAttribute(std::string name);
		void removeAttribute(std::string name);
		//getAttributes;
		int getNumAttributes();
		RmlElementJSWrapper getFocusLeafNode();
		//RmlContextJSWrapper getContext(); // planning on only have one context, so should not need this
		std::string getTagName();
		std::string getId();
		void setId(std::string id);
		float getAbsoluteLeft();
		float getAbsoluteTop();
		float getClientLeft();
		float getClientTop();
		float getClientWidth();
		float getClientHeight();
		RmlElementJSWrapper getOffsetParent();
		float getOffsetLeft();
		float getOffsetTop();
		float getOffsetWidth();
		float getOffsetHeight();
		float getScrollLeft();
		void setScrollLeft(float scrollLeft);
		float getScrollTop();
		void setScrollTop(float scrollTop);
		float getScrollWidth();
		float getScrollHeight();
		//RmlElementStyleJSWrapper getStyle
		RmlDocumentJSWrapper getOwnerDocument();
		RmlElementJSWrapper getParentNode();
		RmlElementJSWrapper closest(std::string selector);
		RmlElementJSWrapper getNextSibling();
		RmlElementJSWrapper getPreviousSibling();
		RmlElementJSWrapper getFirstChild();
		RmlElementJSWrapper getLastChild();
		RmlElementJSWrapper getChild(int index);
		int getNumChildren(bool includeNonDomElements = false);
		std::string getInnerRML();
		void setInnerRML(std::string rml);
		bool focus();
		void blur();
		void click();
		// Not including overload for addEventListener
		void addEventListener(std::string eventType, v8::Local<v8::Function> func, bool inCapturePhase = false);
		// Not includiong overload for removeEventListener
		void removeEventListener(std::string eventType, v8::Local<v8::Function> func, bool inCapturePhase = false);
		//bool dispatchEvent(std::string eventType, const Dictionary& params, bool interruptible, bool bubbles = true) // relies on variant
		// void scrollIntoView(scrollIntoViewOptions opts)
		void scrollIntoView(bool alignWithTop = true);
		//void scrollTo(std::vector<float>, scrollbehavior)
		//RmlElementJSWrapper appendChild // relies on ElementPtr
		// RmlElementJSWrapper insertBefore // relies on ElementPtr
		// ElementPtr replaceChild // relies on ElementPtr
		// ElementPtr removeChild // relies on ElementPtr
		bool hasChildNodes();
		RmlElementJSWrapper getElementById(std::string id);
		std::vector<RmlElementJSWrapper> getElementsByTagName(std::string tagName);
		std::vector<RmlElementJSWrapper> getElementsByClassName(std::string className);
		RmlElementJSWrapper querySelector(std::string selector);
		std::vector<RmlElementJSWrapper> querySelectorAll(std::string selector);

		// for internal use only
		bool isValid();
		Rml::Element* getUnderlyingElement();
	private:
		Rml::Element* _element;
	};
}