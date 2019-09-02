#pragma once

#ifdef GX_BUILD_DLL
#define GX_DLL __declspec(dllexport)
#else
#define GX_DLL __declspec(dllimport)
#endif

#define GX_ENABLE_ASSERTS;

#ifdef GX_ENABLE_ASSERTS
#define GX_ASSERT(x, ...) { if(!(x)) { GX_FERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define GXE_ASSERT(x, ...) { if(!(x)) { GXE_FERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define GX_ASSERT(x, ...)
#define GXE_ASSERT(x, ...)
#endif