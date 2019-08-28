#pragma once

#ifdef GX_BUILD_DLL
#define GX_DLL __declspec(dllexport)
#else
#define GX_DLL __declspec(dllimport)
#endif 
