#pragma once

#include "header_wrappers/rmlui_wrapper.h"

namespace JS {
	class RmlElementDeletionListenerPlugin : public Rml::Plugin {
	public:
		RmlElementDeletionListenerPlugin() = default;
		virtual ~RmlElementDeletionListenerPlugin() = default;
		virtual void OnElementDestroy(Rml::Element* element) override;
	};
}