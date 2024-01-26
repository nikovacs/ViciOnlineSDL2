#pragma once

#include <RmlUi/Core.h>
#include <string>
#include <unordered_map>

namespace JS {
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
		float resolveNumericProperty(std::string property);
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
		// not even half way, come back to this when we need it

		// for internal use only
		bool isValid();
		Rml::Element* getUnderlyingElement();
	private:
		Rml::Element* _element;
		static std::unordered_map<Rml::Element*, int> _elementRefCounts;
	};
}