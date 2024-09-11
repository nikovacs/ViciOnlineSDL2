if (PORT MATCHES "^(libpq)$")
    # libpq requires the default x64-windows triplet
    unset(VCPKG_CHAINLOAD_TOOLCHAIN_FILE)
endif()
