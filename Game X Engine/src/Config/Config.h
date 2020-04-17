#pragma once
//ADD "_ENABLE_EXTENDED_ALIGNED_STORAGE" to preprocessor Bullet engine extended aligned storage
/* add those to preprocessor [TODO Static linking has some errors]
GX_BUILD_DLL
_WINDLL
//runtime should be multithreaded debug -Mtd [for bullet to work]
*//*else add
GX_BUILD_STATIC
*/
#ifndef GX_BUILD_STATIC
#if defined(GX_BUILD_DLL)
#define GX_DLL __declspec(dllexport)
#elif defined(GX_GET_DLL)
#define GX_DLL __declspec(dllimport)
#endif
#else
#define GX_DLL 
#endif // DEBUG

//Config 
#define USING_GLM_MATHS
#define USING_OPENGL
#define RENDER_DEBUG
#define USING_EDITOR_CAMERA
#define GX_ENABLE_ASSERTS
//IMGUI IS USING GLAD
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

 

#ifdef GX_ENABLE_ASSERTS
#define GX_ASSERT(x, ...) { if(!(x)) { GX_FERROR("Assertion Failed: {0}\n{1}", __VA_ARGS__); __debugbreak(); } }
#define GXE_ASSERT(x, ...) { if(!(x)) { GXE_FERROR("Assertion Failed: {0}\n{1}", __VA_ARGS__); __debugbreak(); } }
#else
#define GX_ASSERT(x, ...)
#define GXE_ASSERT(x, ...)
#endif

#ifdef USING_OPENGL
#ifdef RENDER_DEBUG //ASSERTION FOR RENDERING INITIALIZATION CALLS AND DRAW CALLS
#define GL_CALL(x) while(glGetError()!=GL_NO_ERROR);x;while(GLenum error=glGetError())\
GXE_ASSERT(!error,"FATAL GLERROR",error)
#else
#define GL_CALL(x) x
#endif 
#endif