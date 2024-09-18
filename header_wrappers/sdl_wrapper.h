/**
 * Disable all warnings generated from SDL.h for Clang variants
 * Supports: clang-cl (Windows), Apple Clang, and Clang (Linux)
 */
#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#if defined(__clang__)
// This covers Apple Clang, standard Clang, and clang-cl
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#else
#error This header is designed for Clang-based compilers only.
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif // SDL_WRAPPER_H