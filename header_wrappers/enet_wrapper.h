/**
 * Disable all warnings generated from SDL.h for Clang variants
 * Supports: clang-cl (Windows), Apple Clang, and Clang (Linux)
 */
#ifndef ENET_WRAPPER_H
#define ENET_WRAPPER_H

#if defined(__clang__)
// This covers Apple Clang, standard Clang, and clang-cl
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#else
#error This header is designed for Clang-based compilers only.
#endif

#include <enet/enet.h>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif // ENET_WRAPPER_H