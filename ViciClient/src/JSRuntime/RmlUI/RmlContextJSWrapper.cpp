#include <RmlUi/Core.h>
#include <string>
#include <vector>
#include <v8pp/context.hpp>
#include "ViciClient/include/JSRuntime/RmlUI/RmlContextJSWrapper.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/RmlDocumentJSWrapper.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/ViciAsyncDocumentLoader.hpp"

namespace JS {
	RmlContextJSWrapper::RmlContextJSWrapper(Rml::Context& ctx, v8::Local<v8::Context> v8Ctx) : _ctx{ ctx } {
		_isolate = v8Ctx->GetIsolate();
	}

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

	v8::Local<v8::Promise> RmlContextJSWrapper::loadDocument(std::string docPath) {
		v8::Isolate::Scope isolateScope{ _isolate };
		return JS::ViciAsyncDocumentLoader::loadDocumentAsync(docPath, _isolate->GetCurrentContext());
	}

	v8::Local<v8::Promise> RmlContextJSWrapper::loadDocumentFromString(std::string document, std::string docName) {
		v8::Isolate::Scope isolateScope{ _isolate };
		return JS::ViciAsyncDocumentLoader::loadDocumentAsyncFromMemory(document, docName, _isolate->GetCurrentContext());
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
		return RmlElementJSWrapper{ _ctx.GetElementAtPoint(pointVec, ignoreElement.getUnderlyingElement(), element.getUnderlyingElement()) };
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
		_ctx.SetDocumentsBaseTag(baseTag);
	}

	std::string RmlContextJSWrapper::getDocumentsBaseTag() {
		return _ctx.GetDocumentsBaseTag();
	}

	Rml::Context& RmlContextJSWrapper::getUnderlyingContext() {
		return _ctx;
	}
}