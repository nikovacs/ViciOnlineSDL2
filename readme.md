NOTE

Requires Visual Studio 2022
- Install C++ Desktop Tools
- Install Cmake Tools
- Install Windows SDK

PASS -DVCPKG_TARGET_TRIPLET and -DVCPKG_OVERLAY_TRIPLETS on cmd line -DCMAKE_MAKE_PROGRAM=ninja

-msse2 compiler flag is x86_amd64 specific

remove cxx flags to reduce clutter
- remove any warning or error generation on subdirectories that are not maintained by me
- remove /showIncludes to prevent insane clutter that breaks my terminal