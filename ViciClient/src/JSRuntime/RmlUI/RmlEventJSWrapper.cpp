#include "ViciClient/include/JSRuntime/RmlUI/RmlEventJSWrapper.hpp"

namespace JS {
	RmlEventJSWrapper::RmlEventJSWrapper(Rml::Event& event) : _ev(event) {}

	void RmlEventJSWrapper::setCurrentElement(RmlElementJSWrapper el) {
		_ev.SetCurrentElement(el.getUnderlyingElement());
	}

	RmlElementJSWrapper RmlEventJSWrapper::getCurrentElement() {
		return RmlElementJSWrapper(_ev.GetCurrentElement());
	}

	RmlElementJSWrapper RmlEventJSWrapper::getTargetElement() {
		return RmlElementJSWrapper(_ev.GetTargetElement());
	}

	std::string RmlEventJSWrapper::getType() {
		return _ev.GetType();
	}

	void RmlEventJSWrapper::stopPropagation() {
		_ev.StopPropagation();
	}

	void RmlEventJSWrapper::stopImmediatePropagation() {
		_ev.StopImmediatePropagation();
	}

	bool RmlEventJSWrapper::isInterruptible() {
		return _ev.IsInterruptible();
	}

	bool RmlEventJSWrapper::isPropagating() {
		return _ev.IsPropagating();
	}

	bool RmlEventJSWrapper::isImmediatePropagating() {
		return _ev.IsImmediatePropagating();
	}

	std::vector<float> RmlEventJSWrapper::getUnprojectedMouseScreenPos() const {
		Rml::Vector2f pos{ _ev.GetUnprojectedMouseScreenPos() };
		return { pos[0], pos[1] };
	}
}