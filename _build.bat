@echo off
setlocal enabledelayedexpansion

if "%~1"=="" goto MissingArgs
if "%~2"=="" goto MissingArgs
goto main

:MissingArgs
echo Usage: %0 "C:\path\to\ViciOnlineSDL2" "cmake command" "cmake build commnad"
echo Example: %0 "W:\VSSource\repos\ViciOnlineSDL2" "cmake -G Ninja -DCMAKE_BUILD_TYPE=Release .." "cmake --build C:\path\to\build --config Release .."
exit /b 1

:: Check whether VSINSTALLDIR is set, if not cause an error
if not defined VSINSTALLDIR (
    echo VSINSTALLDIR is not set. Please set this environment variable to the directory where Visual Studio is installed.
    exit /b 1
)

:: See whether VSINSTALLDIR ends in a backslash, if not cause an error
if not "%VSINSTALLDIR:~-1%"=="\" (
    echo Usage: %0 "C:\path\to\ViciOnlineSDL2" "cmake command"
    echo VSINSTALLDIR does not end in a backslash. Please set this environment variable to the directory where Visual Studio is installed.
    exit /b 1
)

:: Run vcvars64.bat
:main
call "%VSINSTALLDIR%VC\Auxiliary\Build\vcvarsall.bat" x86_amd64

:: Setup compilers variables using parameter 1 as the base path, 2 as the cmake command
set "vici_path=%~1"
set "cmake_command=%~2"
if not "%~3"=="" (
    set "cmake_build_command=%~3"
)

:: Convert to short path to avoid "input line too long" errors
for %%I in ("%vici_path%") do set "vici_short_path=%%~sI"

:: Set compiler paths using short path
set "CXX=%vici_short_path%\third_party\clang\clang-cl.exe"
set "CC=%CXX%"

:: Running CMake
cd "%vici_path%"
set "modified_cmake_command=%cmake_command% -DCMAKE_C_COMPILER="%CC%" -DCMAKE_CXX_COMPILER="%CXX%""
echo Running cmake command: %modified_cmake_command%
%modified_cmake_command%

:: Running CMake build
if defined cmake_build_command (
    echo Running cmake build command: %cmake_build_command%
    %cmake_build_command%
)

endlocal