# Requirements to build
## Windows
- [Visual Studio 2022](https://visualstudio.microsoft.com/vs/) or Build Tools
  - Install C++ Desktop Tools
  - Install Cmake Tools
  - Install Windows SDK
- git-bash installed at C:\Program Files
  - Comes with standard [git](https://git-scm.com/downloads) installation
  - (default installation path)
- [Ninja](https://github.com/ninja-build/ninja/releases)
  - Possibly comes with visual studio installation
  - Ensure Ninja is on your `$PATH`
## Other
- [homebrew](https://brew.sh/)
## All platforms
- [Python3](https://www.python.org/downloads/)

# Instructions to build
## Prerequisites
- Must be connected to Nik's VPN to access the artifact server containing dependencies like v8, clang for Windows builds
## Windows
- `py .\build.py --help` to see flags
- Example: `py .\build.py -d` to build debug mode
## Other
- `python3 ./build.py --help` to see flags
- Example: `python3 ./build.py -r` tp build release mode

Final executables can be found in the `out` directory