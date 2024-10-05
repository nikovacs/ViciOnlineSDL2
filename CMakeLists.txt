cmake_minimum_required(VERSION 3.29.6)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
file(TO_CMAKE_PATH "${CMAKE_TOOLCHAIN_FILE}" CMAKE_TOOLCHAIN_FILE)

# If CMAKE_BUILD_TYPE is not set, set it to Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

project(
    ViciOnline
    VERSION 0.0.1
    DESCRIPTION "Vici Online Client and Server with Dependencies"
    LANGUAGES CXX
)

if (WIN32)
    # Set _LIBCPP_HARDENING_MODE depending on the build type
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(libcpp_hardening _LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG)
    else()
        set(libcpp_hardening _LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_FAST)
    endif()

    add_compile_definitions(WIN32) #v8pp uses this to determine if it's on Windows
    set(LIBCXX_PATH "${CMAKE_CURRENT_LIST_DIR}/third_party/libcxx")
    set(LIBCXX_INCLUDES "-I\"${LIBCXX_PATH}/include\" -I\"${LIBCXX_PATH}/include.19\"")
    set(LIBCXX_FLAGS "${LIBCXX_INCLUDES} -fuse-ld=lld /clang:-fexceptions /EHsc /Zc:dllexportInlines- -msse2 -D${libcpp_hardening}")

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(LIBCXX_FLAGS "${LIBCXX_FLAGS} -DV8_ENABLE_CHECKS")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LIBCXX_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${LIBCXX_FLAGS}")

    set(LIBCXX_LINK_FLAGS "\"${LIBCXX_PATH}/${CMAKE_BUILD_TYPE}/lib/libc++.dll.lib\"")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LIBCXX_LINK_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LIBCXX_LINK_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} ${LIBCXX_LINK_FLAGS}")
    add_definitions(-DNOMINMAX) #v8pp uses std::min and std::max which are mistakened for macros in Windows
endif()

add_compile_options(-Wno-unused-command-line-argument)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the C standard to C17
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(v8 SHARED IMPORTED)
add_library(v8_libbase SHARED IMPORTED)
add_library(v8_libplatform SHARED IMPORTED)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/third_party/v8/include") # an attempt to get warnings to stop generating for these files
if (WIN32)
    set_target_properties(v8 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/third_party/v8/${CMAKE_BUILD_TYPE}/bin/v8.dll"
        IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/third_party/v8/${CMAKE_BUILD_TYPE}/lib/v8.dll.lib"
    )
    set_target_properties(v8_libbase PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/third_party/v8/${CMAKE_BUILD_TYPE}/bin/v8_libbase.dll"
        IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/third_party/v8/${CMAKE_BUILD_TYPE}/lib/v8_libbase.dll.lib"
    )
    set_target_properties(v8_libplatform PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/third_party/v8/${CMAKE_BUILD_TYPE}/bin/v8_libplatform.dll"
        IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/third_party/v8/${CMAKE_BUILD_TYPE}/lib/v8_libplatform.dll.lib"
    )
elseif (APPLE)
    set_target_properties(v8 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/third_party/v8/lib/libv8.dylib"
    )
    set_target_properties(v8_libbase PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/third_party/v8/lib/libv8_libbase.dylib"
    )
    set_target_properties(v8_libplatform PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/third_party/v8/lib/libv8_libplatform.dylib"
    )
endif()

find_package(nlohmann_json CONFIG REQUIRED)

add_library(
    ViciCore STATIC
    "ViciCore/include/Attributes/Attributes.hpp"
    "ViciCore/src/Attributes/Attributes.cpp" 
    "ViciCore/include/Entities/Entity.hpp"
    "ViciCore/src/Entities/Entity.cpp"
    "ViciCore/include/JSRuntime/Script.hpp"
    "ViciCore/src/JSRuntime/Script.cpp"
    "ViciCore/include/JSRuntime/ScriptLoader.hpp"
    "ViciCore/src/JSRuntime/ScriptLoader.cpp"
    "ViciCore/include/Networking/AssetTransfer.hpp"
    "ViciCore/src/Networking/AssetTransfer.cpp"
    "ViciCore/include/Networking/SimplePacket.hpp"
    "ViciCore/src/Networking/SimplePacket.cpp"
    "ViciCore/include/Networking/UdpHost.hpp"
    "ViciCore/src/Networking/UdpHost.cpp"
    "ViciCore/include/Utility/AssetCache.hpp"
    "ViciCore/src/Utility/AssetCache.cpp"
    "ViciCore/include/Utility/TryLockGuard.hpp"
    "ViciCore/src/Utility/TryLockGuard.cpp"
    
    "ViciCore/include/Networking/UdpChannels.hpp"
    "ViciCore/include/Utility/base64.hpp"
)
target_include_directories(
    ViciCore PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/v8pp"
)
# To match the build settings of the V8 project
target_compile_definitions(
    ViciCore PUBLIC
    V8_COMPRESS_POINTERS
    V8_ENABLE_SANDBOX
    V8_31BIT_SMIS_ON_64BIT_ARCH
)
target_link_libraries(ViciCore PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(ViciCore PUBLIC v8 v8_libbase v8_libplatform)

add_executable(
    ViciServer
    "ViciServer/src/main.cpp"
    "ViciServer/include/ViciServer.hpp"
    "ViciServer/src/ViciServer.cpp"
    "ViciServer/include/Database/DbAsyncQueryRunner.hpp"
    "ViciServer/src/Database/DbAsyncQueryRunner.cpp"
    "ViciServer/include/Database/DbConnectionPool.hpp"
    "ViciServer/src/Database/DbConnectionPool.cpp"
    "ViciServer/include/Entities/ServerPlayer.hpp"
    "ViciServer/src/Entities/ServerPlayer.cpp"
    "ViciServer/include/JSRuntime/DbJSWrapper.hpp"
    "ViciServer/src/JSRuntime/DbJSWrapper.cpp"
    "ViciServer/include/JSRuntime/ServerPlayerJSWrapper.hpp"
    "ViciServer/src/JSRuntime/ServerPlayerJSWrapper.cpp"
    "ViciServer/include/JSRuntime/ServerScriptLoader.hpp"
    "ViciServer/src/JSRuntime/ServerScriptLoader.cpp"
    "ViciServer/include/Networking/AssetBroker.hpp"
    "ViciServer/src/Networking/AssetBroker.cpp"
    "ViciServer/include/Networking/ServerPlayerManager.hpp"
    "ViciServer/src/Networking/ServerPlayerManager.cpp"
    "ViciServer/include/Networking/UdpServer.hpp"
    "ViciServer/src/Networking/UdpServer.cpp"
)
add_subdirectory(v8pp)
target_compile_options(v8pp PRIVATE -w)
target_link_libraries(ViciServer PRIVATE v8pp)
target_link_libraries(ViciServer PRIVATE ViciCore)

find_package(libpqxx CONFIG REQUIRED)
target_link_libraries(ViciServer PRIVATE libpqxx::pqxx)

find_package(unofficial-enet CONFIG REQUIRED)
target_link_libraries(ViciServer PRIVATE unofficial::enet::enet)

add_executable(
    ViciClient
    "ViciClient/src/main.cpp"
    "ViciClient/include/ViciClient.hpp"
    "ViciClient/src/ViciClient.cpp"
    "ViciClient/include/Camera.hpp"
    "ViciClient/src/Camera.cpp"
    "ViciClient/include/PlayerInfo.hpp"
    "ViciClient/src/PlayerInfo.cpp"
    "ViciClient/include/TimeManager.hpp"
    "ViciClient/include/Animations/Animation.hpp"
    "ViciClient/src/Animations/Animation.cpp"
    "ViciClient/include/Animations/Frame.hpp"
    "ViciClient/src/Animations/Frame.cpp"
    "ViciClient/include/Animations/FrameDirection.hpp"
    "ViciClient/src/Animations/FrameDirection.cpp"
    "ViciClient/include/Animations/Gottimation.hpp"
    "ViciClient/src/Animations/Gottimation.cpp"
    "ViciClient/include/Animations/IAnimation.hpp"
    "ViciClient/include/AssetTypes/Texture.hpp"
    "ViciClient/src/AssetTypes/Texture.cpp"
    "ViciClient/include/Comparators/CaseInsensitiveComparator.hpp"
    "ViciClient/src/Comparators/CaseInsensitiveComparator.cpp"
    "ViciClient/include/Entities/ClientPlayer.hpp"
    "ViciClient/src/Entities/ClientPlayer.cpp"
    "ViciClient/include/Entities/EntityAnimated.hpp"
    "ViciClient/src/Entities/EntityAnimated.cpp"
    "ViciClient/include/Entities/EntityDrawable.hpp"
    "ViciClient/src/Entities/EntityDrawable.cpp"
    "ViciClient/include/Entities/EntityTexture.hpp"
    "ViciClient/src/Entities/EntityTexture.cpp"
    "ViciClient/include/Entities/NetworkedPlayer.hpp"
    "ViciClient/src/Entities/NetworkedPlayer.cpp"
    "ViciClient/include/GUI/RmlUi_FileInterface.h"
    "ViciClient/src/GUI/RmlUi_FileInterface.cpp"
    "ViciClient/include/GUI/RmlUi_Platform_SDL.h"
    "ViciClient/src/GUI/RmlUi_Platform_SDL.cpp"
    "ViciClient/include/GUI/RmlUi_Renderer_SDL.h"
    "ViciClient/src/GUI/RmlUi_Renderer_SDL.cpp"
    "ViciClient/include/GUI/ViciGuiHost.hpp"
    "ViciClient/src/GUI/ViciGuiHost.cpp"
    "ViciClient/include/Handlers/KeyboardInputHandler.hpp"
    "ViciClient/src/Handlers/KeyboardInputHandler.cpp"
    "ViciClient/include/Handlers/MouseInputHandler.hpp"
    "ViciClient/src/Handlers/MouseInputHandler.cpp"
    "ViciClient/include/JSRuntime/RmlUI/RmlContextJSWrapper.hpp"
    "ViciClient/src/JSRuntime/RmlUI/RmlContextJSWrapper.cpp"
    "ViciClient/include/JSRuntime/RmlUI/RmlDocumentJSWrapper.hpp"
    "ViciClient/src/JSRuntime/RmlUI/RmlDocumentJSWrapper.cpp"
    "ViciClient/include/JSRuntime/RmlUI/RmlElementJSWrapper.hpp"
    "ViciClient/src/JSRuntime/RmlUI/RmlElementJSWrapper.cpp"
    "ViciClient/include/JSRuntime/RmlUI/RmlEventJSWrapper.hpp"
    "ViciClient/src/JSRuntime/RmlUI/RmlEventJSWrapper.cpp"
    "ViciClient/include/JSRuntime/RmlUI/RmlElementDeletionListenerPlugin.hpp"
    "ViciClient/src/JSRuntime/RmlUI/RmlElementDeletionListenerPlugin.cpp"
    "ViciClient/include/JSRuntime/RmlUI/ViciAsyncDocumentLoader.hpp"
    "ViciClient/src/JSRuntime/RmlUI/ViciAsyncDocumentLoader.cpp"
    "ViciClient/include/JSRuntime/RmlUI/ViciJSEventListenerHandler.hpp"
    "ViciClient/src/JSRuntime/RmlUI/ViciJSEventListenerHandler.cpp"
    "ViciClient/include/JSRuntime/AbstractPlayerJSWrapper.hpp"
    "ViciClient/src/JSRuntime/AbstractPlayerJSWrapper.cpp"
    "ViciClient/include/JSRuntime/ClientPlayerJSWrapper.hpp"
    "ViciClient/src/JSRuntime/ClientPlayerJSWrapper.cpp"
    "ViciClient/include/JSRuntime/NetworkedPlayerJSWrapper.hpp"
    "ViciClient/src/JSRuntime/NetworkedPlayerJSWrapper.cpp"
    "ViciClient/include/JSRuntime/ClientScriptLoader.hpp"
    "ViciClient/src/JSRuntime/ClientScriptLoader.cpp"
    "ViciClient/include/Levels/Level.hpp"
    "ViciClient/src/Levels/Level.cpp"
    "ViciClient/include/Levels/MapLevel.hpp"
    "ViciClient/src/Levels/MapLevel.cpp"
    "ViciClient/include/Levels/SingleLevel.hpp"
    "ViciClient/src/Levels/SingleLevel.cpp"
    "ViciClient/include/Networking/AssetManager.hpp"
    "ViciClient/src/Networking/AssetManager.cpp"
    "ViciClient/include/Networking/ClientPlayerManager.hpp"
    "ViciClient/src/Networking/ClientPlayerManager.cpp"
    "ViciClient/include/Networking/LevelWatchingManager.hpp"
    "ViciClient/src/Networking/LevelWatchingManager.cpp"
    "ViciClient/include/Networking/NetworkAsset.hpp"
    "ViciClient/include/Networking/UdpChannelMap.hpp"
    "ViciClient/include/Networking/UdpClient.hpp"
    "ViciClient/src/Networking/UdpClient.cpp"
    "ViciClient/include/Scenes/GameScene.hpp"
    "ViciClient/src/Scenes/GameScene.cpp"
    "ViciClient/include/Scenes/LoginScene.hpp"
    "ViciClient/src/Scenes/LoginScene.cpp"
    "ViciClient/include/Scenes/Scene.hpp"
    "ViciClient/src/Scenes/Scene.cpp"
    "ViciClient/include/Scenes/SceneManager.hpp"
    "ViciClient/src/Scenes/SceneManager.cpp"
)
target_link_libraries(ViciClient PRIVATE ViciCore)
target_link_libraries(ViciClient PRIVATE v8pp)
target_link_libraries(ViciClient PRIVATE unofficial::enet::enet)
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(
    ViciClient
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(
    ViciClient 
    PRIVATE 
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
)
add_subdirectory(RmlUi)
target_compile_options(RmlCore PRIVATE -w)
target_compile_options(RmlDebugger PRIVATE -w)
target_include_directories(ViciClient PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/RmlUi/Include")
target_link_libraries(ViciClient PRIVATE
    RmlCore
    RmlDebugger
)
target_include_directories(ViciClient PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third_party/XPlatCppSdk/external/jsoncpp/include")
target_include_directories(ViciClient PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third_party/XPlatCppSdk/code/include")
if (WIN32)
    add_subdirectory(third_party/XPlatCppSdk/build/windows XPlatCppSdk)
    target_compile_options(XPlatCppSdkWindows PRIVATE -w)
    target_compile_options(jsoncpp_lib PRIVATE -w)
    target_compile_options(jsoncpp_lib_static PRIVATE -w)
    target_link_libraries(ViciClient PRIVATE XPlatCppSdkWindows)
elseif (APPLE)
    add_custom_target(
        jsoncpp
        COMMENT "Build jsoncpp for Apple"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND xcodebuild
        -project ${CMAKE_CURRENT_SOURCE_DIR}/third_party/XPlatCppSdk/external/jsoncpp-ios-build/lib_json.xcodeproj
        -configuration ${CMAKE_BUILD_TYPE}
        SDKROOT=macosx15.0
        BUILD_DIR=${CMAKE_CURRENT_BINARY_DIR}/playfab
        ONLY_ACTIVE_ARCH=NO
        -arch x86_64 -arch arm64
    )
    add_dependencies(ViciClient jsoncpp)
    # build XPlatCppSdk/Apple/XPlatCpp/XPlatCpp.xcodeproj
    add_custom_target(
        XPlatCppSdk
        COMMENT "Build XPlatCppSdk for Apple"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND xcodebuild
        -project ${CMAKE_CURRENT_SOURCE_DIR}/third_party/XPlatCppSdk/build/Apple/XPlatCpp/XPlatCpp.xcodeproj
        -configuration ${CMAKE_BUILD_TYPE}
        -scheme XPlatCpp_MacOS
        BUILD_DIR=${CMAKE_CURRENT_BINARY_DIR}/playfab
        ONLY_ACTIVE_ARCH=NO
        -arch x86_64 -arch arm64
    )
    add_dependencies(XPlatCppSdk jsoncpp)
    add_dependencies(ViciClient XPlatCppSdk)
    target_link_libraries(
        ViciClient PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/playfab/${CMAKE_BUILD_TYPE}/liblib_json.a
        ${CMAKE_CURRENT_BINARY_DIR}/playfab/${CMAKE_BUILD_TYPE}/libXPlatCpp_MacOS.a
    )
endif()

if(APPLE)
    # fix library paths
    set(dylibs_to_fix
        libv8.dylib libv8_libbase.dylib libv8_libplatform.dylib
    )
    foreach(dylib ${dylibs_to_fix})
        add_custom_command(
            TARGET ViciClient POST_BUILD
            COMMAND install_name_tool -change @@HOMEBREW_PREFIX@@/opt/v8/libexec/${dylib} @executable_path/${dylib} $<TARGET_FILE:ViciClient>
        )
        add_custom_command(
            TARGET ViciServer POST_BUILD
            COMMAND install_name_tool -change @@HOMEBREW_PREFIX@@/opt/v8/libexec/${dylib} @executable_path/${dylib} $<TARGET_FILE:ViciServer>
        )
    endforeach()

    # copy v8 dylibs to the output directory
    add_custom_command(
        TARGET ViciCore POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/v8/lib
        $<TARGET_FILE_DIR:ViciCore>
    )
    set(RML_DYLIBS libRmlCore.6.0.dylib libRmlDebugger.6.0.dylib)
    # copy each of these to the output dir
    foreach(dylib ${RML_DYLIBS})
        add_custom_command(
            TARGET ViciClient POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/RmlUi/${dylib}
            $<TARGET_FILE_DIR:ViciClient>
        )
    endforeach()
endif()

add_custom_command(
    TARGET ViciClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/ViciServer/assets
    $<TARGET_FILE_DIR:ViciServer>/assets
)
add_custom_command(
    TARGET ViciServer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/ViciServer/serverOptions.json
    $<TARGET_FILE_DIR:ViciServer>
)

if (WIN32)
    add_custom_command(
        TARGET ViciCore POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libcxx/${CMAKE_BUILD_TYPE}/bin
        $<TARGET_FILE_DIR:ViciCore>
    )
    # copy v8 dylibs to the output directory
    add_custom_command(
        TARGET ViciCore POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/v8/${CMAKE_BUILD_TYPE}/bin
        $<TARGET_FILE_DIR:ViciCore>
    )
    set(RML_DLLs RmlCore.dll RmlDebugger.dll)
    # copy each of these to the output dir
    foreach(dll ${RML_DLLs})
        add_custom_command(
            TARGET ViciClient POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/RmlUi/${dll}
            $<TARGET_FILE_DIR:ViciClient>
        )
    endforeach()
endif()

