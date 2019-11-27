#include "pch.h"

#include "GLRenderer.h"

namespace gx {
	void GLRenderer::init() {
		gx::OpenGLContext::init(); GXE_INFO("OpenGL Context initialized using GLAD as Loader!");
		GXE_PRINT(gx::OpenGLContext::getGLVersion());
		GXE_PRINT(gx::OpenGLContext::getGPUName());
		GXE_PRINT(gx::OpenGLContext::getVendorName());
	}


	void GLRenderer::begin()
	{
		gx::OpenGLContext::begin();
	}

	void GLRenderer::draw(uint32_t nElements, RenderType type)
	{
		GL_CALL(glDrawElements(type, nElements, GL_UNSIGNED_INT, 0));
	}
	void GLRenderer::end()
	{
		gx::OpenGLContext::end();
	}

}