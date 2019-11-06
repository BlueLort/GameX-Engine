#pragma once
#include "Config.h"
#include "GLLoader/glad.h"
#include "Window/GXWindow.h"
#include "Shader/GLShaderManager.h"


namespace gx {
	class GXWindow;
	class GX_DLL OpenGLContext
	{
	public:
		static void init();
		static void begin();
		static void end();
		inline static SDL_GLContext getGLContext() { return glContext; }
		inline static const GLubyte* getVendorName() { return glGetString(GL_VENDOR); }
		inline static const GLubyte* getGLVersion() { return glGetString(GL_VERSION); }
		inline static const GLubyte* getGPUName() { return glGetString(GL_RENDERER); }
		inline static void setViewPort(uint32_t Width, uint32_t Height) { glViewport(0, 0, Width, Height); }
	private:
		static SDL_GLContext glContext;
	};

}
