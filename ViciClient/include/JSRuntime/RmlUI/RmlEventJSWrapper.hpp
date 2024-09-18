#pragma once

#include "header_wrappers/rmlui_wrapper.h"
#include <vector>
#include "ViciClient/include/JSRuntime/RmlUI/RmlElementJSWrapper.hpp"

namespace JS {
	class RmlEventJSWrapper {
	public:
		RmlEventJSWrapper(Rml::Event& ev);
		//RmlEventPhaseJSWrapper getPhase();
		//void setPhase
		void setCurrentElement(RmlElementJSWrapper el);
		RmlElementJSWrapper getCurrentElement();
		RmlElementJSWrapper getTargetElement();
		std::string getType();
		//RmlEventIdJSWrapper getId();
		void stopPropagation();
		void stopImmediatePropagation();
		bool isInterruptible();
		bool isPropagating();
		bool isImmediatePropagating();
		//bool operator==(const RmlEventJSWrapper other) const;
		//bool operator==(RmlEventIdJSWrapper id) const;
		//T GetParameter
		//GetParameters
		std::vector<float> getUnprojectedMouseScreenPos() const;
	private:
		Rml::Event& _ev;
	};
}