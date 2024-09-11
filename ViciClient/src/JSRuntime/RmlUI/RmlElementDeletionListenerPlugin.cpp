#include "ViciClient/include/JSRuntime/RmlUI/RmlElementDeletionListenerPlugin.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/ViciJSEventListenerHandler.hpp"

namespace JS {
	void RmlElementDeletionListenerPlugin::OnElementDestroy(Rml::Element* element) {
		ViciJSEventListenerHandler::onElementDeleted(element);
	}
}