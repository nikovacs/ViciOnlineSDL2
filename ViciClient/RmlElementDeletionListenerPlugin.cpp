#include "RmlElementDeletionListenerPlugin.hpp"
#include "ViciJSEventListenerHandler.hpp"

namespace JS {
	void RmlElementDeletionListenerPlugin::OnElementDestroy(Rml::Element* element) {
		ViciJSEventListenerHandler::onElementDeleted(element);
	}
}