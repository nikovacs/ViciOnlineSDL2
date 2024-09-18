#include <string>
#include <vector>
#include "header_wrappers/rmlui_wrapper.h"
#include "header_wrappers/v8_wrapper.h"
#include <iostream>
#include "ViciClient/include/JSRuntime/RmlUI/ViciJSEventListenerHandler.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/RmlElementJSWrapper.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/RmlDocumentJSWrapper.hpp"

namespace JS {
	RmlElementJSWrapper::RmlElementJSWrapper(Rml::Element* element) : _element{ element } {}

	RmlElementJSWrapper::~RmlElementJSWrapper() {}

	void RmlElementJSWrapper::setClass(std::string className, bool activate) {
		_element->SetClass(className, activate);
	}

	bool RmlElementJSWrapper::isClassSet(std::string className) {
		return _element->IsClassSet(className);
	}

	void RmlElementJSWrapper::setClassNames(std::string classNames) {
		_element->SetClassNames(classNames);
	}

	std::string RmlElementJSWrapper::getClassNames() {
		return _element->GetClassNames();
	}

	std::string RmlElementJSWrapper::getAddress(bool includePseudoClasses, bool includeParents) {
		return _element->GetAddress(includePseudoClasses, includeParents);
	}

	void RmlElementJSWrapper::setOffset(std::vector<float> offset, RmlElementJSWrapper offsetParent, bool offsetFixed) {
		Rml::Vector2f offsetVec{ offset[0], offset[1] };
		_element->SetOffset(offsetVec, offsetParent.getUnderlyingElement(), offsetFixed);
	}

	/*std::vector<int> RmlElementJSWrapper::getRelativeOffset(std::string area) {
		return _element->GetRelativeOffset(area);
	}*/

	/*std::vector<int> RmlElementJSWrapper::getAbsoluteOffset(std::string area) {
		_element->GetAbsoluteOffset(area);
	}*/

	int RmlElementJSWrapper::getNumBoxes() {
		return _element->GetNumBoxes();
	}

	float RmlElementJSWrapper::getBaseline() {
		return _element->GetBaseline();
	}

	bool RmlElementJSWrapper::isPointWithinElement(float x, float y) {
		return _element->IsPointWithinElement({ x, y });
	}

	bool RmlElementJSWrapper::isVisible(bool includeAncestors) {
		return _element->IsVisible(includeAncestors);
	}

	float RmlElementJSWrapper::getZIndex() {
		return _element->GetZIndex();
	}

	bool RmlElementJSWrapper::setProperty(std::string name, std::string value) {
		return _element->SetProperty(name, value);
	}

	void RmlElementJSWrapper::removeProperty(std::string name) {
		_element->RemoveProperty(name);
	}

	/*float RmlElementJSWrapper::resolveNumericProperty(std::string property) {
		return _element->ResolveNumericValue();
	}*/

	std::vector<float> RmlElementJSWrapper::getContainingBlock() {
		Rml::Vector2f block{ _element->GetContainingBlock() };
		return { block.x, block.y };
	}

	float RmlElementJSWrapper::getLineHeight() {
		return _element->GetLineHeight();
	}

	bool RmlElementJSWrapper::project(std::vector<float> point) {
		Rml::Vector2f pointVec{ point[0], point[1] };
		return _element->Project(pointVec);
	}

	void RmlElementJSWrapper::setPseudoClass(std::string pseudoClass, bool activate) {
		_element->SetPseudoClass(pseudoClass, activate);
	}

	bool RmlElementJSWrapper::isPseudoClassSet(std::string pseudoClass) {
		return _element->IsPseudoClassSet(pseudoClass);
	}

	bool RmlElementJSWrapper::arePseudoClassesSet(std::vector<std::string> pseudoClasses) {
		return _element->ArePseudoClassesSet(pseudoClasses);
	}

	std::vector<std::string> RmlElementJSWrapper::getActivePseudoClasses() {
		return _element->GetActivePseudoClasses();
	}

	void RmlElementJSWrapper::setAttribute(std::string name, std::string value) {
		_element->SetAttribute(name, value);
	}

	bool RmlElementJSWrapper::hasAttribute(std::string name) {
		return _element->HasAttribute(name);
	}

	void RmlElementJSWrapper::removeAttribute(std::string name) {
		_element->RemoveAttribute(name);
	}

	int RmlElementJSWrapper::getNumAttributes() {
		return _element->GetNumAttributes();
	}

	RmlElementJSWrapper RmlElementJSWrapper::getFocusLeafNode() {
		return RmlElementJSWrapper{ _element->GetFocusLeafNode() };
	}

	std::string RmlElementJSWrapper::getTagName() {
		return _element->GetTagName();
	}

	std::string RmlElementJSWrapper::getId() {
		return _element->GetId();
	}

	void RmlElementJSWrapper::setId(std::string id) {
		_element->SetId(id);
	}

	float RmlElementJSWrapper::getAbsoluteLeft() {
		return _element->GetAbsoluteLeft();
	}

	float RmlElementJSWrapper::getAbsoluteTop() {
		return _element->GetAbsoluteTop();
	}

	float RmlElementJSWrapper::getClientLeft() {
		return _element->GetClientLeft();
	}

	float RmlElementJSWrapper::getClientTop() {
		return _element->GetClientTop();
	}

	float RmlElementJSWrapper::getClientWidth() {
		return _element->GetClientWidth();
	}

	float RmlElementJSWrapper::getClientHeight() {
		return _element->GetClientHeight();
	}

	RmlElementJSWrapper RmlElementJSWrapper::getOffsetParent() {
		return RmlElementJSWrapper{ _element->GetOffsetParent() };
	}

	float RmlElementJSWrapper::getOffsetLeft() {
		return _element->GetOffsetLeft();
	}

	float RmlElementJSWrapper::getOffsetTop() {
		return _element->GetOffsetTop();
	}

	float RmlElementJSWrapper::getOffsetWidth() {
		return _element->GetOffsetWidth();
	}

	float RmlElementJSWrapper::getOffsetHeight() {
		return _element->GetOffsetHeight();
	}

	float RmlElementJSWrapper::getScrollLeft() {
		return _element->GetScrollLeft();
	}

	void RmlElementJSWrapper::setScrollLeft(float scrollLeft) {
		_element->SetScrollLeft(scrollLeft);
	}

	float RmlElementJSWrapper::getScrollTop() {
		return _element->GetScrollTop();
	}

	void RmlElementJSWrapper::setScrollTop(float scrollTop) {
		return _element->SetScrollTop(scrollTop);
	}

	float RmlElementJSWrapper::getScrollWidth() {
		return _element->GetScrollWidth();
	}

	float RmlElementJSWrapper::getScrollHeight() {
		return _element->GetScrollHeight();
	}

	RmlDocumentJSWrapper RmlElementJSWrapper::getOwnerDocument() {
		return RmlDocumentJSWrapper{ _element->GetOwnerDocument() };
	}

	RmlElementJSWrapper RmlElementJSWrapper::getParentNode() {
		return RmlElementJSWrapper{ _element->GetParentNode() };
	}

	RmlElementJSWrapper RmlElementJSWrapper::closest(std::string selector) {
		return RmlElementJSWrapper{ _element->Closest(selector) };
	}

	RmlElementJSWrapper RmlElementJSWrapper::getNextSibling() {
		return RmlElementJSWrapper{ _element->GetNextSibling() };
	}

	RmlElementJSWrapper RmlElementJSWrapper::getPreviousSibling() {
		return RmlElementJSWrapper{ _element->GetPreviousSibling() };
	}

	RmlElementJSWrapper RmlElementJSWrapper::getFirstChild() {
		return RmlElementJSWrapper{ _element->GetFirstChild() };
	}

	RmlElementJSWrapper RmlElementJSWrapper::getLastChild() {
		return RmlElementJSWrapper{ _element->GetLastChild() };
	}

	RmlElementJSWrapper RmlElementJSWrapper::getChild(int index) {
		return RmlElementJSWrapper{ _element->GetChild(index) };
	}

	int RmlElementJSWrapper::getNumChildren(bool includeNonDomElements) {
		return _element->GetNumChildren(includeNonDomElements);
	}

	std::string RmlElementJSWrapper::getInnerRML() {
		return _element->GetInnerRML();
	}

	void RmlElementJSWrapper::setInnerRML(std::string rml) {
		_element->SetInnerRML(rml);
	}

	bool RmlElementJSWrapper::focus() {
		return _element->Focus();
	}

	void RmlElementJSWrapper::blur() {
		_element->Blur();
	}

	void RmlElementJSWrapper::click() {
		_element->Click();
	}

	void RmlElementJSWrapper::addEventListener(std::string type, v8::Local<v8::Function> func, bool inCapturePhase) {
		ViciJSEventListenerHandler::addEventListener(_element, type, func, inCapturePhase);
	}

	void RmlElementJSWrapper::removeEventListener(std::string type, v8::Local<v8::Function> func, bool inCapturePhase) {
		ViciJSEventListenerHandler::removeEventListener(_element, type, func, inCapturePhase);
	}

	void RmlElementJSWrapper::scrollIntoView(bool alignWithTop) {
		_element->ScrollIntoView(alignWithTop);
	}

	bool RmlElementJSWrapper::hasChildNodes() {
		return _element->HasChildNodes();
	}
	
	RmlElementJSWrapper RmlElementJSWrapper::getElementById(std::string id) {
		return RmlElementJSWrapper{ _element->GetElementById(id) };
	}

	std::vector<RmlElementJSWrapper> RmlElementJSWrapper::getElementsByTagName(std::string tagName) {
		Rml::Vector<Rml::Element*> elements{};
		_element->GetElementsByTagName(elements, tagName);
		std::vector<RmlElementJSWrapper> wrappers{};
		for (auto element : elements) {
			wrappers.push_back(RmlElementJSWrapper{ element });
		}
		return wrappers;
	}

	std::vector<RmlElementJSWrapper> RmlElementJSWrapper::getElementsByClassName(std::string className) {
		Rml::Vector<Rml::Element*> elements{};
		_element->GetElementsByClassName(elements, className);
		std::vector<RmlElementJSWrapper> wrappers{};
		for (auto element : elements) {
			wrappers.push_back(RmlElementJSWrapper{ element });
		}
		return wrappers;
	}

	RmlElementJSWrapper RmlElementJSWrapper::querySelector(std::string selector) {
		return RmlElementJSWrapper{ _element->QuerySelector(selector) };
	}

	std::vector<RmlElementJSWrapper> RmlElementJSWrapper::querySelectorAll(std::string selector) {
		Rml::Vector<Rml::Element*> elements{};
		_element->QuerySelectorAll(elements, selector);
		std::vector<RmlElementJSWrapper> wrappers{};
		for (auto element : elements) {
			wrappers.push_back(RmlElementJSWrapper{ element });
		}
		return wrappers;
	}

	bool RmlElementJSWrapper::isValid() {
		return _element != nullptr;
	}

	Rml::Element* RmlElementJSWrapper::getUnderlyingElement() {
		return _element;
	}
}