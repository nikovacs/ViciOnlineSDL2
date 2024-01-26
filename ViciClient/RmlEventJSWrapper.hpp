#pragma once

#include <RmlUi/Core.h>
#include "RmlElementJSWrapper.hpp"
#include <vector>

namespace JS {
	class RmlEventJSWrapper {
	public:
		RmlEventJSWrapper(Rml::Event& ev);
		virtual ~RmlEventJSWrapper();
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