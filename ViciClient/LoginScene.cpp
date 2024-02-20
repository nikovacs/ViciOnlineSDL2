#include "LoginScene.hpp"
#include <iostream>
#include "../ViciEngine/base64.hpp"
#include <playfab/PlayFabApiSettings.h>
#include <playfab/PlayFabSettings.h>
#include <playfab/PlayFabClientApi.h>
#include <playfab/PlayFabClientDataModels.h>
#include <playfab/PlayFabError.h>

#if _WIN32
#include <Windows.h>
#endif // _WIN32

namespace Scenes {
    LoginButtonListener::LoginButtonListener(std::function<void(std::string_view)>& onLoginCallback) : _callbackOnProcess{ onLoginCallback } {}

    void LoginButtonListener::ProcessEvent(Rml::Event& event) {
        auto userField = event.GetCurrentElement()->GetOwnerDocument()->GetElementById("username-field");

        Rml::String username = "";
        if (userField) {
            Rml::Variant* variant = userField->GetAttribute("value");
            if (variant) {
				username = variant->Get<Rml::String>();
			}
        }
        if (!username.empty()) {
            _callbackOnProcess(username);
        }
    }

    ContinueAsGuestButtonListener::ContinueAsGuestButtonListener(LoginScene* parent) : _parent{ parent } {}
    void ContinueAsGuestButtonListener::ProcessEvent(Rml::Event& event) {
		_parent->loginAsGuest();
	}

    LoginScene::LoginScene(std::string_view sceneName, std::function<void(std::string_view)> onLoginCallback) :
        Scene{ sceneName },
        _onLoginCallback{ onLoginCallback },
        _loginButtonListener{ _onLoginCallback },
        _continueAsGuestButtonListener{ this }
    {

        Rml::ElementDocument* document = getContext().LoadDocument("__vici_internal__loginScene.html");
        document->GetElementById("login-btn")->AddEventListener("click", &_loginButtonListener);
        document->GetElementById("continue-as-guest-btn")->AddEventListener("click", &_continueAsGuestButtonListener);
        document->Show();
	}

	void LoginScene::update() {
		Scene::update();
        PlayFab::PlayFabClientAPI::Update();
	}

	void LoginScene::render(SDL_Renderer* renderer) {
        Scene::render(renderer);
	}

    void LoginScene::loginAsGuest() {
        std::string uuid{};
        bool result = getCustomDeviceId(uuid);
        if (!result) {
            std::cerr << "Failed to get custom device id" << std::endl;
            // TODO put an error message on the login scene
            return;
        }
        PlayFab::ClientModels::LoginWithCustomIDRequest request;
        request.CustomId = uuid;
        request.CreateAccount = true;
        

        PlayFab::PlayFabClientAPI::LoginWithCustomID(request,
            [this](const PlayFab::ClientModels::LoginResult& result, void* customData) {
				std::cout << "Logged in as guest" << std::endl;
				//_onLoginCallback(result.PlayFabId);
                std::cout << "id: " << result.PlayFabId << std::endl;
                _onLoginCallback(result.PlayFabId);
			},
			[this](const PlayFab::PlayFabError& error, void* customData) {
				std::cerr << "Failed to login as guest: " << error.ErrorMessage << std::endl;
				// TODO put an error message on the login scene
			}
        );
    }

    bool LoginScene::getCustomDeviceId(std::string& out) {
#if _WIN32
        // Buffer to store SMBIOS data
        BYTE* buffer = nullptr;
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
        std::string SMBIOS_UUID(reinterpret_cast<char*>(buffer + 0x10), 16);
        out = base64::to_base64(SMBIOS_UUID);
        // Clean up
        delete[] buffer;
        std::cout << "SMBIOS UUID: " << out << std::endl;
#else
        throw std::runtime_error("Not implemented for this platform");
#endif // _WIN32
        return true;
    }
}