#pragma once
#include "Config.h"
#include "Window/GXWindow.h"
#include "glad/glad.h"
namespace gx {
	class GXWindow;
	class GX_DLL OpenGLContext
	{
	public:
		static void init();
		inline static SDL_GLContext getGLContext() { return glContext; }
		inline static const GLubyte* getVendorName() { return glGetString(GL_VENDOR); }
		inline static const GLubyte* getGLVersion() { return glGetString(GL_VERSION); }
		inline static const GLubyte* getGPUName() { return glGetString(GL_RENDERER); }
	private:
		static SDL_GLContext glContext;
	};

}
