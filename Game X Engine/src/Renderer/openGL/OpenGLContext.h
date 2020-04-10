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
		inline static SDL_GLContext getGLContext() { return glContext; }
		inline static const GXuint8* getVendorName() { return glGetString(GL_VENDOR); }
		inline static const GXuint8* getGLVersion() { return glGetString(GL_VERSION); }
		inline static const GXuint8* getGPUName() { return glGetString(GL_RENDERER); }
		inline static void setViewPort(GXuint32 Width, GXuint32 Height) { glViewport(0, 0, Width, Height); }
	private:
		static SDL_GLContext glContext;
	};

}
