# Vici Online Dev Setup (Windows)
### Get the VM Virtual Disk from Nik & set up in either VirtualBox(most likely to work) or VMWare
### Open Visual Studio and clone the repo from https://github.com/nikovacs/ViciOnlineSDL2.git
### Dependencies
* VCPKG (Already installed on the VM)
* Boost (via VCPKG)
* RmlUI (via VCPKG)
* Enet (via VCPKG)
* SDL2 (via VCPKG)
* SDL2-Image (via VCPKG)
* v8/v8pp (Alreayd installed on the VM)
* PlayFab (Submodule)
* PostgreSQL (install v16 on vm)
* libpqxx (via VCPKG)

* Open powershell, and navigate to ~/vcpkg
* Run the following commands
  * `./vcpkg.exe install boost` (This takes the longest and is the largest. Currently working to remove this dependency)
  * `./vcpkg.exe install rmlui` (Embeddable implementation of HTML + CSS)
  * `./vcpkg.exe install enet`  (Used for UDP Networking)
  * `./vcpkg.exe install sdl2`  (Displaying to the screen)
  * `./vcpkg.exe install sdl2-image`
  * `./vcpkg.exe install nlohmann-json` (C++ JSON objects)
  * `./vcpkg.exe install libpqxx`
