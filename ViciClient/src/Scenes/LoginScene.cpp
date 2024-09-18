#include "ViciClient/include/Scenes/LoginScene.hpp"
#include "ViciClient/include/PlayerInfo.hpp"
#include "ViciCore/include/Utility/base64.hpp"
#include "header_wrappers/playfab_wrapper.h"
#include <RmlUi/Debugger.h>
#include <iostream>
#include <regex>

#ifdef _WIN32
#include "header_wrappers/windows_wrapper.h"
#endif // _WIN32

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#endif // __APPLE__

namespace Scenes {
LoginButtonListener::LoginButtonListener(LoginScene *parent) : _parent{parent} {}

void LoginButtonListener::ProcessEvent(Rml::Event &event) {
    auto updateError = [&parent = _parent](std::string_view error) {
        Rml::Element *errorElement = parent->_loginSceneDoc->GetElementById("error");
        errorElement->SetInnerRML(error.data());
    };

    std::string email{};
    auto emailField = event.GetCurrentElement()->GetOwnerDocument()->GetElementById("email-field");
    if (emailField) {
        auto variant = emailField->GetAttribute("value");
        if (variant) {
            email = variant->Get<Rml::String>();
        }
    }

    if (email.empty()) {
        updateError("Email field cannot be empty");
        return;
    }

    if (!_parent->_isValidEmail(email)) {
        updateError("Invalid email");
        return;
    }

    std::string password{};
    auto passwordField = event.GetCurrentElement()->GetOwnerDocument()->GetElementById("password-field");
    if (passwordField) {
        auto variant = passwordField->GetAttribute("value");
        if (variant) {
            password = variant->Get<Rml::String>();
        }
    }

    if (password.empty()) {
        updateError("Password cannot be empty");
        return;
    }

    PlayFab::ClientModels::LoginWithEmailAddressRequest request;
    request.Password = password;
    request.Email = email;

    PlayFab::PlayFabClientAPI::LoginWithEmailAddress(request, [this](const PlayFab::ClientModels::LoginResult &result, [[maybe_unused]] void *customData) {
				PlayerInfo::playerId = result.PlayFabId;
				// get player username
                PlayFab::ClientModels::GetAccountInfoRequest request;
                PlayFab::PlayFabClientAPI::GetAccountInfo(request,
                    [this](const PlayFab::ClientModels::GetAccountInfoResult& result, [[maybe_unused]] void* customData) {
                        PlayerInfo::username = result.AccountInfo->Username;
                        _parent->_onLoginCallback();
                    }
                ); }, [updateError](const PlayFab::PlayFabError &error, [[maybe_unused]] void *customData) { updateError(error.ErrorMessage); });
}

ContinueAsGuestButtonListener::ContinueAsGuestButtonListener(LoginScene *parent) : _parent{parent} {}
void ContinueAsGuestButtonListener::ProcessEvent([[maybe_unused]] Rml::Event &event) {
    _parent->_loginAsGuest();
}

RegisterNowButtonListener::RegisterNowButtonListener(LoginScene *parent) : _parent{parent} {}
void RegisterNowButtonListener::ProcessEvent([[maybe_unused]] Rml::Event &event) {
    _parent->_registerSceneDoc->Show();
    _parent->_loginSceneDoc->Hide();
}

BackButtonListener::BackButtonListener(LoginScene *parent) : _parent{parent} {}
void BackButtonListener::ProcessEvent([[maybe_unused]] Rml::Event &event) {
    _parent->_registerSceneDoc->Hide();
    _parent->_loginSceneDoc->Show();
}

RegisterButtonListener::RegisterButtonListener(LoginScene *parent) : _parent{parent} {}
void RegisterButtonListener::ProcessEvent([[maybe_unused]] Rml::Event &event) {
    auto updateError = [&parent = _parent](std::string_view error) {
        Rml::Element *errorElement = parent->_registerSceneDoc->GetElementById("error");
        errorElement->SetInnerRML(error.data());
    };

    Rml::Element *emailField = _parent->_registerSceneDoc->GetElementById("email-field");
    Rml::Element *confirmEmailField = _parent->_registerSceneDoc->GetElementById("confirm-email-field");
    Rml::Element *usernameField = _parent->_registerSceneDoc->GetElementById("username-field");
    Rml::Element *passwordField = _parent->_registerSceneDoc->GetElementById("password-field");
    Rml::Element *confirmPasswordField = _parent->_registerSceneDoc->GetElementById("confirm-password-field");

    std::string email{};
    auto variant = emailField->GetAttribute("value");
    if (variant)
        email = variant->Get<Rml::String>();
    std::string confirmEmail{};
    variant = confirmEmailField->GetAttribute("value");
    if (variant)
        confirmEmail = variant->Get<Rml::String>();
    std::string username{};
    variant = usernameField->GetAttribute("value");
    if (variant)
        username = variant->Get<Rml::String>();
    std::string password{};
    variant = passwordField->GetAttribute("value");
    if (variant)
        password = variant->Get<Rml::String>();
    std::string confirmPassword{};
    variant = confirmPasswordField->GetAttribute("value");
    if (variant)
        confirmPassword = variant->Get<Rml::String>();

    if (!LoginScene::_isValidEmail(email)) {
        updateError("Invalid email");
        return;
    }

    if (email != confirmEmail) {
        updateError("Emails do not match");
        return;
    }

    if (password.empty()) {
        updateError("Password cannot be empty");
        return;
    }

    if (password != confirmPassword) {
        updateError("Passwords do not match");
        return;
    }

    if (username.empty()) {
        updateError("Username cannot be empty");
        return;
    }

    _parent->_addEmailUsernamePassword(email, username, password);
}

LoginScene::LoginScene(std::string_view sceneName, std::function<void()> onLoginCallback)
    : Scene{sceneName},
      _loginButtonListener{this},
      _onLoginCallback{onLoginCallback},
      _continueAsGuestButtonListener{this},
      _registerNowButtonListener{this},
      _backButtonListener{this},
      _registerButtonListener{this} {
#if _DEBUG
    Rml::Debugger::Initialise(&getContext());
    Rml::Debugger::SetContext(&getContext());
    Rml::Debugger::SetVisible(true);
#endif // _DEBUG

    _registerSceneDoc = getContext().LoadDocument("__vici_internal__registerScene.html");
    _registerSceneDoc->GetElementById("back-btn")->AddEventListener("click", &_backButtonListener);
    _registerSceneDoc->GetElementById("register-btn")->AddEventListener("click", &_registerButtonListener);

    _loginSceneDoc = getContext().LoadDocument("__vici_internal__loginScene.html");
    _loginSceneDoc->GetElementById("login-btn")->AddEventListener("click", &_loginButtonListener);
    _loginSceneDoc->GetElementById("continue-as-guest-btn")->AddEventListener("click", &_continueAsGuestButtonListener);
    _loginSceneDoc->GetElementById("register-btn")->AddEventListener("click", &_registerNowButtonListener);
    _loginSceneDoc->Show();
}

LoginScene::~LoginScene() {
    if (_loginSceneDoc)
        _loginSceneDoc->Close();
    if (_registerSceneDoc)
        _registerSceneDoc->Close();
}

void LoginScene::update() {
    Scene::update();
    PlayFab::PlayFabClientAPI::Update();
}

void LoginScene::render(SDL_Renderer *renderer) {
    Scene::render(renderer);
}

void LoginScene::_loginAsGuest(bool goToGameScene) {
    std::string uuid{};
    bool result = _getCustomDeviceId(uuid);
    if (!result) {
        std::cerr << "Failed to get custom device id" << std::endl;
        // TODO put an error message on the login scene
        return;
    }
    PlayFab::ClientModels::LoginWithCustomIDRequest request;
    request.CustomId = uuid;
    request.CreateAccount = true;

    PlayFab::PlayFabClientAPI::LoginWithCustomID(request, [this, goToGameScene](const PlayFab::ClientModels::LoginResult &result, [[maybe_unused]] void *customData) {
				std::cout << "Logged in as guest" << std::endl;
                std::cout << "id: " << result.PlayFabId << std::endl;
                PlayerInfo::playerId = result.PlayFabId;
                PlayerInfo::username = result.PlayFabId;
                if (goToGameScene) {
                    _onLoginCallback();
                } }, [](const PlayFab::PlayFabError &error, [[maybe_unused]] void *customData) {
        std::cerr << "Failed to login as guest: " << error.ErrorMessage << std::endl;
        // TODO put an error message on the login scene
    });
}

bool LoginScene::_getCustomDeviceId(std::string &out) {
#ifdef _WIN32
    // Buffer to store SMBIOS data
    BYTE *buffer = nullptr;
    DWORD bufferSize = 0;

    // Call GetSystemFirmwareTable to get the required buffer size
    bufferSize = GetSystemFirmwareTable('RSMB', 0, nullptr, 0);
    if (bufferSize == 0) {
        return false;
    }

    // Allocate memory for the buffer
    buffer = new BYTE[bufferSize];

    // Call GetSystemFirmwareTable again to retrieve the SMBIOS data
    bufferSize = GetSystemFirmwareTable('RSMB', 0, buffer, bufferSize);
    if (bufferSize == 0) {
        delete[] buffer;
        return false;
    }
    // Extract the SMBIOS UUID from the buffer
    std::string SMBIOS_UUID(reinterpret_cast<char *>(buffer + 0x10), 16);
    out = base64::to_base64(SMBIOS_UUID);
    // Clean up
    delete[] buffer;
    std::cout << "SMBIOS UUID: " << out << std::endl;
#elif __APPLE__
    io_service_t platformExpert = IOServiceGetMatchingService(kIOMainPortDefault, IOServiceMatching("IOPlatformExpertDevice"));
    if (!platformExpert) {
        return false;
    }

    CFTypeRef serialNumberAsCFString = IORegistryEntryCreateCFProperty(platformExpert, CFSTR(kIOPlatformUUIDKey), kCFAllocatorDefault, 0);
    IOObjectRelease(platformExpert);
    if (!serialNumberAsCFString) {
        return false;
    }

    CFStringRef serialNumberAsString = (CFStringRef)serialNumberAsCFString;
    char buffer[1024];
    if (!CFStringGetCString(serialNumberAsString, buffer, sizeof(buffer), kCFStringEncodingUTF8)) {
        CFRelease(serialNumberAsCFString);
        return false;
    }

    out = base64::to_base64(buffer);
    CFRelease(serialNumberAsCFString);
    std::cout << "SMBIOS UUID: " << out << std::endl;
#else
    throw std::runtime_error("Not implemented for this platform");
#endif // _WIN32
    return true;
}

void LoginScene::_addEmailUsernamePassword(std::string_view email, std::string_view username, std::string_view password) {
    // necessary to keep guest account progress
    _loginAsGuest(false);

    PlayFab::ClientModels::AddUsernamePasswordRequest request;
    request.Email = email;
    request.Password = password;
    request.Username = username;

    PlayFab::PlayFabClientAPI::AddUsernamePassword(
        request,
        [this](const PlayFab::ClientModels::AddUsernamePasswordResult &result, [[maybe_unused]] void *customData) {
            _unlinkCustomId();
            _registerSceneDoc->Hide();
            _loginSceneDoc->Show();
            PlayerInfo::username = result.Username;
            _onLoginCallback();
        });
}

void LoginScene::_unlinkCustomId() {
    PlayFab::ClientModels::UnlinkCustomIDRequest request;
    _getCustomDeviceId(request.CustomId);

    PlayFab::PlayFabClientAPI::UnlinkCustomID(
        request,
        []([[maybe_unused]] const PlayFab::ClientModels::UnlinkCustomIDResult &result, [[maybe_unused]] void *customData) {
            std::cout << "Unlinked custom id" << std::endl;
        },
        [](const PlayFab::PlayFabError &error, [[maybe_unused]] void *customData) {
            std::cerr << "Failed to unlink custom id: " << error.ErrorMessage << std::endl;
        });
}

bool LoginScene::_isValidEmail(std::string_view email) {
    // Regular expression definition
    const std::regex pattern(
        "(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

    // Match the string pattern
    // with regular expression
    return std::regex_match(email.data(), pattern);
}
} // namespace Scenes