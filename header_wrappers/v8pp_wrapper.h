/**
 * Disable all warnings generated from SDL.h for Clang variants
 * Supports: clang-cl (Windows), Apple Clang, and Clang (Linux)
 */
#ifndef V8PP_WRAPPER_H
#define V8PP_WRAPPER_H

#if defined(__clang__)
// This covers Apple Clang, standard Clang, and clang-cl
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#else
#error This header is designed for Clang-based compilers only.
#endif

#include <v8pp/context.hpp>
#include <v8pp/function.hpp>
#include <v8pp/module.hpp>
#include <v8pp/class.hpp>
#include <v8pp/throw_ex.hpp>
#include <v8pp/convert.hpp>
#include <v8pp/call_v8.hpp>
#include <v8pp/json.hpp>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif // V8PP_WRAPPER_H