#include "pch.h"

#include "Renderer.h"

namespace gx {
	void Renderer::init() {
#ifdef USING_OPENGL
		gx::OpenGLContext::init(); GXE_INFO("OpenGL Context initialized using GLAD as Loader!");
		GXE_PRINT(gx::OpenGLContext::getGLVersion());
		GXE_PRINT(gx::OpenGLContext::getGPUName());
		GXE_PRINT(gx::OpenGLContext::getVendorName());
#endif 


	}

	void Renderer::begin()
	{
#ifdef USING_OPENGL
		gx::OpenGLContext::begin();
#endif 
	}
	void Renderer::end()
	{
#ifdef USING_OPENGL
		gx::OpenGLContext::end();
#endif 
	}
	
}