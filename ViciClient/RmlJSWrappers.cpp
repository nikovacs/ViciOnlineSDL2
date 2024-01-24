#include "RmlJSWrappers.hpp"

namespace JS {
	/* BEGIN RML CONTEXT JS WRAPPER */
	RmlContextJSWrapper::RmlContextJSWrapper(Rml::Context& ctx) : _ctx{ ctx } {}

	RmlContextJSWrapper::~RmlContextJSWrapper() {}

	void RmlContextJSWrapper::setDimensions(std::vector<int> dimensions) {
		_ctx.SetDimensions({ dimensions[0], dimensions[1] });
	}

	std::vector<int> RmlContextJSWrapper::getDimensions() {
		Rml::Vector2i dimensions{ _ctx.GetDimensions() };
		return { dimensions.x, dimensions.y };
	}

	void RmlContextJSWrapper::setDensityIndependentPixelRatio(float ratio) {
		_ctx.SetDensityIndependentPixelRatio(ratio);
	}

	float RmlContextJSWrapper::getDensityIndependentPixelRatio() {
		return _ctx.GetDensityIndependentPixelRatio();
	}

	RmlDocumentJSWrapper RmlContextJSWrapper::createDocument(std::string instancerName) {
		return RmlDocumentJSWrapper{ _ctx.CreateDocument(instancerName) };
	}

	RmlDocumentJSWrapper RmlContextJSWrapper::loadDocumentFromString(std::string document, std::string docName) {
		return RmlDocumentJSWrapper{ _ctx.LoadDocumentFromMemory(document, docName) };
	}

	void RmlContextJSWrapper::unloadDocument(RmlDocumentJSWrapper doc) {
		_ctx.UnloadDocument(doc.getUnderlyingDocument());
	}

	void RmlContextJSWrapper::unloadAllDocuments() {
		_ctx.UnloadAllDocuments();
	}

	void RmlContextJSWrapper::enableMouseCursor(bool enable) {
		_ctx.EnableMouseCursor(enable);
	}

	void RmlContextJSWrapper::activateTheme(std::string themeName, bool activate) {
		_ctx.ActivateTheme(themeName, activate);
	}

	bool RmlContextJSWrapper::isThemeActive(std::string themeName) {
		return _ctx.IsThemeActive(themeName);
	}

	RmlDocumentJSWrapper RmlContextJSWrapper::getDocument(std::string docName) {
		return RmlDocumentJSWrapper{ _ctx.GetDocument(docName) };
	}

	RmlElementJSWrapper RmlContextJSWrapper::getHoverElement() {
		return RmlElementJSWrapper{ _ctx.GetHoverElement() };
	}

	RmlElementJSWrapper RmlContextJSWrapper::getFocusElement() {
		return RmlElementJSWrapper{ _ctx.GetFocusElement() };
	}

	RmlElementJSWrapper RmlContextJSWrapper::getRootElement() {
		return RmlElementJSWrapper{ _ctx.GetRootElement() };
	}

	RmlElementJSWrapper RmlContextJSWrapper::getElementAtPoint(std::vector<float> point, RmlElementJSWrapper ignoreElement, RmlElementJSWrapper element) {
		Rml::Vector2f pointVec{ point[0], point[1] };
		return RmlElementJSWrapper{ _ctx.GetElementAtPoint(pointVec, ignoreElement.getUnderlyingElement(), element.getUnderlyingElement())};
	}

	void RmlContextJSWrapper::pullDocumentToFront(RmlDocumentJSWrapper document) {
		_ctx.PullDocumentToFront(document.getUnderlyingDocument());
	}

	void RmlContextJSWrapper::pushDocumentToBack(RmlDocumentJSWrapper document) {
		_ctx.PushDocumentToBack(document.getUnderlyingDocument());
	}

	void RmlContextJSWrapper::unfocusDocument(RmlDocumentJSWrapper document) {
		_ctx.UnfocusDocument(document.getUnderlyingDocument());
	}

	bool RmlContextJSWrapper::processTextInput(std::string input) {
		return _ctx.ProcessTextInput(input);
	}

	bool RmlContextJSWrapper::processMouseMove(int x, int y, int keyModifierState) {
		return _ctx.ProcessMouseMove(x, y, keyModifierState);
	}

	bool RmlContextJSWrapper::processMouseLeave() {
		return _ctx.ProcessMouseLeave();
	}

	bool RmlContextJSWrapper::isMouseInteracting() {
		return _ctx.IsMouseInteracting();
	}

	bool RmlContextJSWrapper::getActiveClipRegion(std::vector<int> origin, std::vector<int> dimensions) {
		Rml::Vector2i originVec{ origin[0], origin[1] };
		Rml::Vector2i dimensionsVec{ dimensions[0], dimensions[1] };
		return _ctx.GetActiveClipRegion(originVec, dimensionsVec);
	}

	void RmlContextJSWrapper::setActiveClipRegion(std::vector<int> origin, std::vector<int> dimensions) {
		Rml::Vector2i originVec{ origin[0], origin[1] };
		Rml::Vector2i dimensionsVec{ dimensions[0], dimensions[1] };
		_ctx.SetActiveClipRegion(originVec, dimensionsVec);
	}

	bool RmlContextJSWrapper::removeDataModel(std::string modelName) {
		return _ctx.RemoveDataModel(modelName);
	}

	void RmlContextJSWrapper::setDocumentsBaseTag(std::string baseTag) {
		_ctx.SetDocumentsBaseTag(baseTag.data());
	}

	std::string RmlContextJSWrapper::getDocumentsBaseTag() {
		return _ctx.GetDocumentsBaseTag();
	}

	Rml::Context& RmlContextJSWrapper::getUnderlyingContext() {
		return _ctx;
	}
	/* END RML CONTEXT JS WRAPPER */

	/* BEGIN RML DOCUMENT JS WRAPPER */
	RmlDocumentJSWrapper::RmlDocumentJSWrapper(Rml::ElementDocument* document) : _document{ document } {}

	RmlDocumentJSWrapper::~RmlDocumentJSWrapper() {}

	void RmlDocumentJSWrapper::setTitle(std::string title) {
		_document->SetTitle(title.data());
	}

	std::string RmlDocumentJSWrapper::getTitle() {
		return _document->GetTitle();
	}

	std::string RmlDocumentJSWrapper::getSourceURL() {
		return _document->GetSourceURL();
	}

	void RmlDocumentJSWrapper::reloadStyleSheet() {
		_document->ReloadStyleSheet();
	}

	void RmlDocumentJSWrapper::pullToFront() {
		_document->PullToFront();
	}

	void RmlDocumentJSWrapper::pushToBack() {
		_document->PushToBack();
	}

	//void RmlDocumentJSWrapper::show(std::string modalFlag, std::string focusFlag) {}

	void RmlDocumentJSWrapper::hide() {
		_document->Hide();
	}

	void RmlDocumentJSWrapper::close() {
		_document->Close();
	}

	bool RmlDocumentJSWrapper::isModal() {
		return _document->IsModal();
	}

	bool RmlDocumentJSWrapper::isValidDocument() {
		return _document != nullptr;
	}

	Rml::ElementDocument* RmlDocumentJSWrapper::getUnderlyingDocument() {
		return _document;
	}
	/* END RML DOCUMENT JS WRAPPER */

	/* BEGIN RML ELEMENT JS WRAPPER */
	RmlElementJSWrapper::RmlElementJSWrapper(Rml::Element* element) : _element{ element } {}

	RmlElementJSWrapper::~RmlElementJSWrapper() {}

	void RmlElementJSWrapper::setClass(std::string className, bool activate) {
		_element->SetClass(className.data(), activate);
	}

	bool RmlElementJSWrapper::isClassSet(std::string className) {
		return _element->IsClassSet(className.data());
	}

	void RmlElementJSWrapper::setClassNames(std::string classNames) {
		_element->SetClassNames(classNames.data());
	}

	std::string RmlElementJSWrapper::getClassNames() {
		return _element->GetClassNames();
	}

	std::string RmlElementJSWrapper::getAddress(bool includePseudoClasses, bool includeParents) {
		return _element->GetAddress(includePseudoClasses, includeParents);
	}

	void RmlElementJSWrapper::setOffset(float x, float y, RmlElementJSWrapper offsetParent, bool offsetFixed) {
		_element->SetOffset({ x, y }, offsetParent.getUnderlyingElement(), offsetFixed);
	}

	/*std::vector<int> RmlElementJSWrapper::getRelativeOffset(std::string area) {
		return _element->GetRelativeOffset(area.data());
	}*/

	/*std::vector<int> RmlElementJSWrapper::getAbsoluteOffset(std::string area) {
		_element->GetAbsoluteOffset(area.data());
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
		return _element->SetProperty(name.data(), value.data());
	}

	void RmlElementJSWrapper::removeProperty(std::string name) {
		_element->RemoveProperty(name.data());
	}

	float RmlElementJSWrapper::resolveNumericProperty(std::string property) {
		return _element->ResolveNumericProperty(property.data());
	}

	std::vector<float> RmlElementJSWrapper::getContainingBlock() {
		Rml::Vector2f block{ _element->GetContainingBlock() };
		return { block.x, block.y };
	}

	float RmlElementJSWrapper::getLineHeight() {
		return _element->GetLineHeight();
	}

	bool RmlElementJSWrapper::project(float x, float y) {
		Rml::Vector2f point{ x, y };
		return _element->Project(point);
	}

	void RmlElementJSWrapper::setPseudoClass(std::string pseudoClass, bool activate) {
		_element->SetPseudoClass(pseudoClass.data(), activate);
	}

	bool RmlElementJSWrapper::isPseudoClassSet(std::string pseudoClass) {
		return _element->IsPseudoClassSet(pseudoClass.data());
	}

	bool RmlElementJSWrapper::arePseudoClassesSet(std::vector<std::string> pseudoClasses) {
		return _element->ArePseudoClassesSet(pseudoClasses);
	}

	std::vector<std::string> RmlElementJSWrapper::getActivePseudoClasses() {
		return _element->GetActivePseudoClasses();
	}

	bool RmlElementJSWrapper::isValidElement() {
		return _element != nullptr;
	}

	Rml::Element* RmlElementJSWrapper::getUnderlyingElement() {
		return _element;
	}
}