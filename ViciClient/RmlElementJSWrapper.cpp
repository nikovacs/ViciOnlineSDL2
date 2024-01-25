#include "RmlElementJSWrapper.hpp"
#include <string>
#include <vector>
#include <RmlUi/Core.h>

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

	float RmlElementJSWrapper::resolveNumericProperty(std::string property) {
		return _element->ResolveNumericProperty(property);
	}

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

	bool RmlElementJSWrapper::isValid() {
		return _element != nullptr;
	}

	Rml::Element* RmlElementJSWrapper::getUnderlyingElement() {
		return _element;
	}
}