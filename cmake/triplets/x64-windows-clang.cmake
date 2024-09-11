set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

# For the following ports, use normal x64-windows triplet
set(x64WindowsPorts
    libpq
)



# Specify the path to your custom toolchain file
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/../toolchains/clang-windows-toolchain.cmake")
set(VCPKG_LOAD_VCVARS_ENV ON) # Setting VCPKG_CHAINLOAD_TOOLCHAIN_FILE deactivates automatic vcvars setup so reenable it!

## Policy settings
# set(VCPKG_POLICY_SKIP_ARCHITECTURE_CHECK enabled)
# set(VCPKG_POLICY_SKIP_DUMPBIN_CHECKS enabled)

# Do not set compiler paths here, do it in the chainloaded toolchain file

include("${CMAKE_CURRENT_LIST_DIR}/../toolchains/clang-windows-port-specialization.cmake")