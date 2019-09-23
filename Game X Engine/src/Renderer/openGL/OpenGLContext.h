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
		static SDL_GLContext getGLContext() { return glContext; }

	private:
		static SDL_GLContext glContext;
	};

}
