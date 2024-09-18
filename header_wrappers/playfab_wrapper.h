/**
 * Disable all warnings generated from SDL.h for Clang variants
 * Supports: clang-cl (Windows), Apple Clang, and Clang (Linux)
 */
#ifndef PLAYFAB_WRAPPER_H
#define PLAYFAB_WRAPPER_H

#if defined(__clang__)
// This covers Apple Clang, standard Clang, and clang-cl
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#else
#error This header is designed for Clang-based compilers only.
#endif

#include <playfab/PlayFabApiSettings.h>
#include <playfab/PlayFabSettings.h>
#include <playfab/PlayFabClientApi.h>
#include <playfab/PlayFabClientDataModels.h>
#include <playfab/PlayFabError.h>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif // PLAYFAB_WRAPPER_H