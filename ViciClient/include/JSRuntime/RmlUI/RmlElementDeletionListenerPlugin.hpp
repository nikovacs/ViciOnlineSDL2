#pragma once

#include <RmlUi/Core.h>

namespace JS {
	class RmlElementDeletionListenerPlugin : public Rml::Plugin {
	public:
		RmlElementDeletionListenerPlugin() = default;
		virtual ~RmlElementDeletionListenerPlugin() = default;
		virtual void OnElementDestroy(Rml::Element* element) override;
	};
}