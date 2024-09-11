#include "ViciClient/include/JSRuntime/RmlUI/RmlDocumentJSWrapper.hpp"

namespace JS {
	RmlDocumentJSWrapper::RmlDocumentJSWrapper(Rml::ElementDocument* document) : RmlElementJSWrapper(document), _document{ document } {}

	RmlDocumentJSWrapper::~RmlDocumentJSWrapper() {}


	void RmlDocumentJSWrapper::setTitle(std::string title) {
		_document->SetTitle(title);
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

	void RmlDocumentJSWrapper::show() {
		_document->Show();
	}

	void RmlDocumentJSWrapper::hide() {
		_document->Hide();
	}

	void RmlDocumentJSWrapper::close() {
		_document->Close();
	}

	bool RmlDocumentJSWrapper::isModal() {
		return _document->IsModal();
	}

	Rml::ElementDocument* RmlDocumentJSWrapper::getUnderlyingDocument() {
		return _document;
	}
}