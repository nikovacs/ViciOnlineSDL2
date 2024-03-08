# Vici Online Dev Setup (Windows)
### Get the VM Virtual Disk from Nik & set up in either VirtualBox(most likely to work) or VMWare
### Open Visual Studio and clone the repo from https://github.com/nikovacs/ViciOnlineSDL2.git
### Dependencies
* VCPKG (Install in ~)
* Boost (via VCPKG)
* Enet (via VCPKG)
* SDL2 (via VCPKG)
* SDL2-Image (via VCPKG)
* nlohmann-json (via VCPKG)
* freetype (via VCPKG, dependency for RmlUi)
* libpqxx (via VCPKG)
* v8/v8pp (submodule)
* PlayFab (submodule)
* RmlUi (submodule)

* Open powershell, and navigate to ~/vcpkg
* Run the following commands
  * `./vcpkg.exe install boost` (This takes the longest and is the largest. Currently working to remove this dependency)
  * `./vcpkg.exe install enet`  (Used for UDP Networking)
  * `./vcpkg.exe install sdl2`  (Displaying to the screen)
  * `./vcpkg.exe install sdl2-image`
  * `./vcpkg.exe install nlohmann-json` (C++ JSON objects)
  * `./vcpkg.exe install freetype`
  * `./vcpkg.exe install libpqxx`
