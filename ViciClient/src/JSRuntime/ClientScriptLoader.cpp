#include "ViciClient/include/JSRuntime/ClientScriptLoader.hpp"
#include "ViciClient/include/Entities/NetworkedPlayer.hpp"
#include "ViciClient/include/Handlers/KeyboardInputHandler.hpp"
#include "ViciClient/include/JSRuntime/ClientPlayerJSWrapper.hpp"
#include "ViciClient/include/JSRuntime/NetworkedPlayerJSWrapper.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/RmlContextJSWrapper.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/RmlDocumentJSWrapper.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/RmlElementJSWrapper.hpp"
#include "ViciClient/include/JSRuntime/RmlUI/RmlEventJSWrapper.hpp"
#include "ViciClient/include/Networking/ClientPlayerManager.hpp"
#include "ViciClient/include/Networking/NetworkAsset.hpp"
#include "ViciClient/include/PlayerInfo.hpp"
#include "ViciClient/include/ViciClient.hpp"
#include "header_wrappers/v8pp_wrapper.h"
#include <map>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <vector>

JS::ClientScriptLoader::ClientScriptLoader() {}

JS::ClientScriptLoader::~ClientScriptLoader() {
    for (auto &script : _scripts) {
        auto *scriptPtr = script.second->getValue();
        if (scriptPtr) {
            scriptPtr->trigger("onUnload");
        }
        // it is important that the scripts in these unique ptrs are released immediately
        // the shut down order of v8 is important
        script.second.release();
    }
}

void JS::ClientScriptLoader::update() {
    {
        std::lock_guard<std::mutex> lock{_scriptsToLoadMutex};
        for (const std::string &script : _scriptsToLoad) {
            loadScript(script);
        }
        _scriptsToLoad.clear();
    }

    {
        std::lock_guard<std::mutex> lock{_scriptsToUnloadMutex};
        for (const std::string &script : _scriptsToUnload) {
            unloadScript(script);
        }
        _scriptsToUnload.clear();
    }

    attemptResolveInProgress();

    for (auto &script : _scripts) {
        script.second->getValue()->trigger("onUpdate");
    }
}

void JS::ClientScriptLoader::attemptResolveInProgress() {
    if (_scriptsInProgress.size() == 0)
        return;

    std::vector<std::string> toRemove{};
    for (auto &[name, script] : _scriptsInProgress) {
        JS::Script *scriptPtr = script->getValue();
        if (scriptPtr) {
            _scripts[name] = std::move(script);
            toRemove.push_back(name);
            scriptPtr->initialize([this](v8pp::context *ctx) -> void { setApiSetupFuncs(ctx); });
            scriptPtr->run();
            scriptPtr->trigger("onLoad");
        }
    }
    for (auto &script : toRemove) {
        _scriptsInProgress.erase(script);
    }
}

void JS::ClientScriptLoader::loadScript(std::string_view fileName) {
    _scriptsInProgress.emplace(fileName, std::make_unique<Networking::NetworkAsset<JS::Script>>(fileName));
}

void JS::ClientScriptLoader::unloadScript(std::string_view fileName) {
    if (_scripts.contains(fileName.data())) {
        auto *scriptPtr = _scripts.at(fileName.data())->getValue();
        if (scriptPtr) {
            scriptPtr->trigger("onUnload");
        }
    }
    _scripts.erase(fileName.data());
}

void JS::ClientScriptLoader::setClientPlayer(Entities::ClientPlayer *pl) {
    _clientPlayer = pl;
}

void JS::ClientScriptLoader::onLoadScript(std::string_view fileName) {
    std::lock_guard<std::mutex> lock{_scriptsToLoadMutex};
    _scriptsToLoad.insert(fileName.data());
}

void JS::ClientScriptLoader::onUnloadScript(std::string_view fileName) {
    std::lock_guard<std::mutex> lock{_scriptsToUnloadMutex};
    _scriptsToUnload.insert(fileName.data());
}

void JS::ClientScriptLoader::setApiSetupFuncs(v8pp::context *ctx) {
    if (!_clientPlayer) {
        throw std::runtime_error{"ClientScriptLoader::getApiSetupFuncs() called before ClientScriptLoader::setClientPlayer()!"};
    }
    exposeStandardFuncs(ctx);
    exposeClientPlayer(ctx);
    exposeKeyboardHandler(ctx);
    exposeLocalAttrs(ctx);
    exposeNetworkedPlayerClass(ctx);
    exposeNetworkPlayerManagerFunctions(ctx);
    exposeRmlUIFunctions(ctx);
}

void JS::ClientScriptLoader::exposeStandardFuncs(v8pp::context *ctx) {
    ctx->function("print", [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        v8::Isolate *isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        if (args.Length() < 1) {
            std::cout << std::endl;
            return;
        }
        for (int i = 0; i < args.Length(); i++) {
            v8::String::Utf8Value str(isolate, args[i]);
            std::cout << *str;

            if (i < args.Length() - 1) {
                std::cout << " ";
            }
        }

        std::cout << std::endl;
    });
}

void JS::ClientScriptLoader::exposeClientPlayer(v8pp::context *ctx) {

	static v8pp::class_<JS::ClientPlayerJSWrapper> clientPlayerClass{ _isolate };
	clientPlayerClass
		.auto_wrap_objects(true)
        .function("getUsername", &JS::ClientPlayerJSWrapper::getUsername)
        .function("getSkeleton", &JS::ClientPlayerJSWrapper::getSkeleton)
        .function("setSkeleton", &JS::ClientPlayerJSWrapper::setSkeleton)
		// .property("username", &JS::ClientPlayerJSWrapper::getUsername)
		// .property("dir", &JS::ClientPlayerJSWrapper::getDir, &JS::ClientPlayerJSWrapper::setDir)
		// .property("ani", &JS::ClientPlayerJSWrapper::getAni, &JS::ClientPlayerJSWrapper::setAni)
		// .property("width", &JS::ClientPlayerJSWrapper::getWidth, &JS::ClientPlayerJSWrapper::setWidth)
		// .property("height", &JS::ClientPlayerJSWrapper::getHeight, &JS::ClientPlayerJSWrapper::setHeight)
		// .property("x", &JS::ClientPlayerJSWrapper::getX, &JS::ClientPlayerJSWrapper::setX)
		// .property("y", &JS::ClientPlayerJSWrapper::getY, &JS::ClientPlayerJSWrapper::setY)
		// .property(
			// "pos",
			// [this]() -> std::vector<int> { return { _clientPlayer->getX(), _clientPlayer->getY() }; },
			// [this](std::vector<int> pos) { _clientPlayer->setPosition(pos[0], pos[1]); }
			// )
		// .property("clientW", &JS::ClientPlayerJSWrapper::getClientW)
		// .property("clientR", &JS::ClientPlayerJSWrapper::getClientR)
		;
	// create a global clientPlayer object for the context
	(void)ctx->global()->Set(ctx->isolate()->GetCurrentContext(), v8pp::to_v8(_isolate, "clientPlayer"), v8pp::to_v8(_isolate, ClientPlayerJSWrapper(_clientPlayer, ctx)));
}

void JS::ClientScriptLoader::exposeKeyboardHandler([[maybe_unused]] v8pp::context *ctx) {
    v8::HandleScope scope{_isolate};

    v8::Local<v8::Function> isKeyDownFunc = v8pp::wrap_function(_isolate, "isKeyDown", &Handlers::KeyboardInputHandler::isKeyDown);
    (void)_isolate->GetCurrentContext()->Global()->Set(_isolate->GetCurrentContext(), v8pp::to_v8(_isolate, "isKeyDown"), isKeyDownFunc);
}

void JS::ClientScriptLoader::exposeLocalAttrs([[maybe_unused]] v8pp::context *ctx) {
    v8::HandleScope scope{_isolate};

    static nlohmann::json localAttrs{};
    v8pp::module localAttrsModule{_isolate};
    localAttrsModule
        .function("setInt", [](std::string key, int64_t value) -> void { localAttrs[key] = value; })
        .function("getInt", [](std::string key) -> int64_t { return localAttrs[key]; })
        .function("setFloat", [](std::string key, double value) -> void { localAttrs[key] = value; })
        .function("getFloat", [](std::string key) -> double { return localAttrs[key]; })
        .function("setString", [](std::string key, std::string value) -> void { localAttrs[key] = value; })
        .function("getString", [](std::string key) -> std::string { return localAttrs[key]; })
        .function("setBool", [](std::string key, bool value) -> void { localAttrs[key] = value; })
        .function("getBool", [](std::string key) -> bool { return localAttrs[key]; })
        .function("getType",
                  [](std::string key) -> std::string {
                      if (!localAttrs.contains(key))
                          return "undefined";
                      if (localAttrs[key].is_number_integer())
                          return "int";
                      if (localAttrs[key].is_number_float())
                          return "float";
                      if (localAttrs[key].is_string())
                          return "string";
                      if (localAttrs[key].is_boolean())
                          return "bool";
                      return "undefined";
                  });
    ctx->module("__VICI__localAttrsModule", localAttrsModule);

    static std::string localAttrsProxyScript{R"(
		const __VICI__INTERNAL__localAttrsHandler = {
			get: function(target, prop, receiver) {
				let type = __VICI__localAttrsModule.getType(prop);
				if (type === "int") {
					return __VICI__localAttrsModule.getInt(prop);
				} else if (type === "float") {
					return __VICI__localAttrsModule.getFloat(prop);
				} else if (type === "string") {
					return __VICI__localAttrsModule.getString(prop);
				} else if (type === "bool") {
					return __VICI__localAttrsModule.getBool(prop);
				} else {
					return undefined;
				}
			},
			set: function(target, prop, value) {
				if (Number.isInteger(value)) {
					__VICI__localAttrsModule.setInt(prop, value);
				} else if (typeof value === "number") {
					__VICI__localAttrsModule.setFloat(prop, value);
				} else if (typeof value === "string") {
					__VICI__localAttrsModule.setString(prop, value);
				} else if (typeof value === "boolean") {
					__VICI__localAttrsModule.setBool(prop, value);
				}
			}
		}
		const localAttrs = new Proxy({}, __VICI__INTERNAL__localAttrsHandler);
	)"};
    v8::Local<v8::String> localAttrsProxyScriptString = v8pp::to_v8(_isolate, localAttrsProxyScript);
    v8::Local<v8::Script> localAttrsProxyScriptCompiled = v8::Script::Compile(_isolate->GetCurrentContext(), localAttrsProxyScriptString).ToLocalChecked();
    localAttrsProxyScriptCompiled->Run(_isolate->GetCurrentContext()).ToLocalChecked();
}

void JS::ClientScriptLoader::exposeNetworkedPlayerClass(v8pp::context *ctx) {
    static v8pp::class_<JS::NetworkedPlayerJSWrapper> networkedPlayerClass{_isolate};
    networkedPlayerClass
        .auto_wrap_objects(true)
        .property("username", &JS::NetworkedPlayerJSWrapper::getUsername)
        // .property("dir", &JS::NetworkedPlayerJSWrapper::getDir)
        // .property("ani", &JS::NetworkedPlayerJSWrapper::getAni)
        .property("width", &JS::NetworkedPlayerJSWrapper::getWidth)
        .property("height", &JS::NetworkedPlayerJSWrapper::getHeight)
        .property("clientW", &JS::NetworkedPlayerJSWrapper::getClientW)
        .property("clientR", &JS::NetworkedPlayerJSWrapper::getClientR);
    ctx->class_("networkedPlayer", networkedPlayerClass);

    ctx->function("getPlayer", [this, ctx](std::string_view username) -> v8::Local<v8::Value> {
        if (PlayerInfo::username == username) {
            return v8pp::class_<JS::ClientPlayerJSWrapper>::create_object(_isolate, _clientPlayer, ctx);
        }
        // TODO - the wrapped player object should put a lock on the underlying networkedPlayer so that it cannot be deleted while the wrapper is still in use
        // Will be important for the wrapper to not be held for longer than the scope of a single function call within javascript
        auto *pl{Networking::ClientPlayerManager::getPlayer(username)}; // pl can be nullptr
        if (pl)
            return v8pp::class_<JS::NetworkedPlayerJSWrapper>::create_object(_isolate, pl, ctx);
        return v8::Null(_isolate);
    });
}

// remove the [[maybe_unused]] attribute from the function signature once implemented
void JS::ClientScriptLoader::exposeNetworkPlayerManagerFunctions([[maybe_unused]] v8pp::context *ctx) {
}

void JS::ClientScriptLoader::exposeRmlUIFunctions(v8pp::context *ctx) {
    static v8pp::class_<JS::RmlContextJSWrapper> rmlContextClass{_isolate};
    static v8pp::class_<JS::RmlElementJSWrapper> rmlElementClass{_isolate};
    static v8pp::class_<JS::RmlDocumentJSWrapper> rmlDocumentClass{_isolate};
    static v8pp::class_<JS::RmlEventJSWrapper> rmlEventClass{_isolate};

    static bool firstTime{true};
    if (firstTime) {

        rmlContextClass
            .auto_wrap_objects(true)
            .function("setDimensions", &JS::RmlContextJSWrapper::setDimensions)
            .function("getDimensions", &JS::RmlContextJSWrapper::getDimensions)
            .function("setDensityIndependentPixelRatio", &JS::RmlContextJSWrapper::setDensityIndependentPixelRatio)
            .function("getDensityIndependentPixelRatio", &JS::RmlContextJSWrapper::getDensityIndependentPixelRatio)
            .function("createDocument", &JS::RmlContextJSWrapper::createDocument)
            .function("loadDocument", &JS::RmlContextJSWrapper::loadDocument)
            .function("loadDocumentFromString", &JS::RmlContextJSWrapper::loadDocumentFromString)
            .function("unloadDocument", &JS::RmlContextJSWrapper::unloadDocument)
            .function("unloadAllDocuments", &JS::RmlContextJSWrapper::unloadAllDocuments)
            .function("enableMouseCursor", &JS::RmlContextJSWrapper::enableMouseCursor)
            .function("activateTheme", &JS::RmlContextJSWrapper::activateTheme)
            .function("isThemeActive", &JS::RmlContextJSWrapper::isThemeActive)
            .function("getDocument", &JS::RmlContextJSWrapper::getDocument)
            .function("getHoverElement", &JS::RmlContextJSWrapper::getHoverElement)
            .function("getFocusElement", &JS::RmlContextJSWrapper::getFocusElement)
            .function("getRootElement", &JS::RmlContextJSWrapper::getRootElement)
            .function("getElementAtPoint", &JS::RmlContextJSWrapper::getElementAtPoint)
            .function("pullDocumentToFront", &JS::RmlContextJSWrapper::pullDocumentToFront)
            .function("pushDocumentToBack", &JS::RmlContextJSWrapper::pushDocumentToBack)
            .function("unfocusDocument", &JS::RmlContextJSWrapper::unfocusDocument)
            .function("processTextInput", &JS::RmlContextJSWrapper::processTextInput)
            .function("processMouseMove", &JS::RmlContextJSWrapper::processMouseMove)
            .function("processMouseLeave", &JS::RmlContextJSWrapper::processMouseLeave)
            .function("isMouseInteracting", &JS::RmlContextJSWrapper::isMouseInteracting)
            .function("getActiveClipRegion", &JS::RmlContextJSWrapper::getActiveClipRegion)
            .function("setActiveClipRegion", &JS::RmlContextJSWrapper::setActiveClipRegion)
            .function("removeDataModel", &JS::RmlContextJSWrapper::removeDataModel)
            .function("setDocumentsBaseTag", &JS::RmlContextJSWrapper::setDocumentsBaseTag)
            .function("getDocumentsBaseTag", &JS::RmlContextJSWrapper::getDocumentsBaseTag);

        rmlElementClass
            .auto_wrap_objects(true)
            .function("setClass", &JS::RmlElementJSWrapper::setClass)
            .function("isClassSet", &JS::RmlElementJSWrapper::isClassSet)
            .function("setClassNames", &JS::RmlElementJSWrapper::setClassNames)
            .function("getClassNames", &JS::RmlElementJSWrapper::getClassNames)
            .function("getAddress", &JS::RmlElementJSWrapper::getAddress)
            .function("setOffset", &JS::RmlElementJSWrapper::setOffset)
            .function("getNumBoxes", &JS::RmlElementJSWrapper::getNumBoxes)
            .function("getBaseline", &JS::RmlElementJSWrapper::getBaseline)
            .function("isPointWithinElement", &JS::RmlElementJSWrapper::isPointWithinElement)
            .function("isVisible", &JS::RmlElementJSWrapper::isVisible)
            .function("getZIndex", &JS::RmlElementJSWrapper::getZIndex)
            .function("setProperty", &JS::RmlElementJSWrapper::setProperty)
            .function("removeProperty", &JS::RmlElementJSWrapper::removeProperty)
            //.function("resolveNumericProperty", &JS::RmlElementJSWrapper::resolveNumericProperty)
            .function("getContainingBlock", &JS::RmlElementJSWrapper::getContainingBlock)
            .function("getLineHeight", &JS::RmlElementJSWrapper::getLineHeight)
            .function("project", &JS::RmlElementJSWrapper::project)
            .function("setPseudoClass", &JS::RmlElementJSWrapper::setPseudoClass)
            .function("isPseudoClassSet", &JS::RmlElementJSWrapper::isPseudoClassSet)
            .function("arePseudoClassesSet", &JS::RmlElementJSWrapper::arePseudoClassesSet)
            .function("getActivePseudoClasses", &JS::RmlElementJSWrapper::getActivePseudoClasses)
            .function("setAttribute", &JS::RmlElementJSWrapper::setAttribute)
            .function("hasAttribute", &JS::RmlElementJSWrapper::hasAttribute)
            .function("removeAttribute", &JS::RmlElementJSWrapper::removeAttribute)
            .function("getNumAttributes", &JS::RmlElementJSWrapper::getNumAttributes)
            .function("getFocusLeafNode", &JS::RmlElementJSWrapper::getFocusLeafNode)
            .function("getTagName", &JS::RmlElementJSWrapper::getTagName)
            .function("getId", &JS::RmlElementJSWrapper::getId)
            .function("setId", &JS::RmlElementJSWrapper::setId)
            .function("getAbsoluteLeft", &JS::RmlElementJSWrapper::getAbsoluteLeft)
            .function("getAbsoluteTop", &JS::RmlElementJSWrapper::getAbsoluteTop)
            .function("getClientLeft", &JS::RmlElementJSWrapper::getClientLeft)
            .function("getClientTop", &JS::RmlElementJSWrapper::getClientTop)
            .function("getClientWidth", &JS::RmlElementJSWrapper::getClientWidth)
            .function("getClientHeight", &JS::RmlElementJSWrapper::getClientHeight)
            .function("getOffsetParent", &JS::RmlElementJSWrapper::getOffsetParent)
            .function("getOffsetLeft", &JS::RmlElementJSWrapper::getOffsetLeft)
            .function("getOffsetTop", &JS::RmlElementJSWrapper::getOffsetTop)
            .function("getOffsetWidth", &JS::RmlElementJSWrapper::getOffsetWidth)
            .function("getOffsetHeight", &JS::RmlElementJSWrapper::getOffsetHeight)
            .function("getScrollLeft", &JS::RmlElementJSWrapper::getScrollLeft)
            .function("setScrollLeft", &JS::RmlElementJSWrapper::setScrollLeft)
            .function("getScrollTop", &JS::RmlElementJSWrapper::getScrollTop)
            .function("setScrollTop", &JS::RmlElementJSWrapper::setScrollTop)
            .function("getScrollWidth", &JS::RmlElementJSWrapper::getScrollWidth)
            .function("getScrollHeight", &JS::RmlElementJSWrapper::getScrollHeight)
            .function("getOwnerDocument", &JS::RmlElementJSWrapper::getOwnerDocument)
            .function("getParentNode", &JS::RmlElementJSWrapper::getParentNode)
            .function("closest", &JS::RmlElementJSWrapper::closest)
            .function("getNextSibling", &JS::RmlElementJSWrapper::getNextSibling)
            .function("getPreviousSibling", &JS::RmlElementJSWrapper::getPreviousSibling)
            .function("getFirstChild", &JS::RmlElementJSWrapper::getFirstChild)
            .function("getLastChild", &JS::RmlElementJSWrapper::getLastChild)
            .function("getChild", &JS::RmlElementJSWrapper::getChild)
            .function("getNumChildren", &JS::RmlElementJSWrapper::getNumChildren)
            .function("getInnerRML", &JS::RmlElementJSWrapper::getInnerRML)
            .function("setInnerRML", &JS::RmlElementJSWrapper::setInnerRML)
            .function("focus", &JS::RmlElementJSWrapper::focus)
            .function("blur", &JS::RmlElementJSWrapper::blur)
            .function("click", &JS::RmlElementJSWrapper::click)
            .function("addEventListener", &JS::RmlElementJSWrapper::addEventListener)
            .function("removeEventListener", &JS::RmlElementJSWrapper::removeEventListener)
            .function("scrollIntoView", &JS::RmlElementJSWrapper::scrollIntoView)
            .function("hasChildNodes", &JS::RmlElementJSWrapper::hasChildNodes)
            .function("getElementById", &JS::RmlElementJSWrapper::getElementById)
            .function("getElementsByTagName", &JS::RmlElementJSWrapper::getElementsByTagName)
            .function("getElementsByClassName", &JS::RmlElementJSWrapper::getElementsByClassName)
            .function("querySelector", &JS::RmlElementJSWrapper::querySelector)
            .function("querySelectorAll", &JS::RmlElementJSWrapper::querySelectorAll);

        rmlDocumentClass
            .auto_wrap_objects(true)
            .inherit<JS::RmlElementJSWrapper>()
            .function("setTitle", &JS::RmlDocumentJSWrapper::setTitle)
            .function("getTitle", &JS::RmlDocumentJSWrapper::getTitle)
            .function("getSourceURL", &JS::RmlDocumentJSWrapper::getSourceURL)
            .function("reloadStyleSheet", &JS::RmlDocumentJSWrapper::reloadStyleSheet)
            .function("pullToFront", &JS::RmlDocumentJSWrapper::pullToFront)
            .function("pushToBack", &JS::RmlDocumentJSWrapper::pushToBack)
            .function("show", &JS::RmlDocumentJSWrapper::show)
            .function("hide", &JS::RmlDocumentJSWrapper::hide)
            .function("close", &JS::RmlDocumentJSWrapper::close)
            .function("isModal", &JS::RmlDocumentJSWrapper::isModal);

        rmlEventClass
            .auto_wrap_objects(true)
            .function("setCurrentElement", &JS::RmlEventJSWrapper::setCurrentElement)
            .function("getCurrentElement", &JS::RmlEventJSWrapper::getCurrentElement)
            .function("getTargetElement", &JS::RmlEventJSWrapper::getTargetElement)
            .function("getType", &JS::RmlEventJSWrapper::getType)
            .function("stopPropagation", &JS::RmlEventJSWrapper::stopPropagation)
            .function("stopImmediatePropagation", &JS::RmlEventJSWrapper::stopImmediatePropagation)
            .function("isInterruptible", &JS::RmlEventJSWrapper::isInterruptible)
            .function("isPropagating", &JS::RmlEventJSWrapper::isPropagating)
            .function("isImmediatePropagating", &JS::RmlEventJSWrapper::isImmediatePropagating)
            .function("getUnprojectedMouseScreenPos", &JS::RmlEventJSWrapper::getUnprojectedMouseScreenPos);

        firstTime = false;
    }

    // create a global rmlContextClass object for the context
    (void)ctx->global()->Set(ctx->isolate()->GetCurrentContext(), v8pp::to_v8(_isolate, "uiContext"), v8pp::to_v8(_isolate, JS::RmlContextJSWrapper(*Rml::GetContext("GameScene"), ctx->impl())));
}