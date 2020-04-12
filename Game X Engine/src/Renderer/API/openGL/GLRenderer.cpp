#include "pch.h"

#include "GLRenderer.h"

namespace gx {
	void GLRenderer::init() {
		gx::OpenGLContext::init(); GXE_INFO("OpenGL Context initialized using GLAD as Loader!");
		GXE_PRINT(gx::OpenGLContext::getGLVersion());
		GXE_PRINT(gx::OpenGLContext::getGPUName());
		GXE_PRINT(gx::OpenGLContext::getVendorName());
	}


	void GLRenderer::draw(GXuint32 nElements, RenderType type,bool isWireFrame)
	{
		//TODO OPTIMIZE THIS
		if (isWireFrame) {
			GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
			GL_CALL(glDrawElements(type, nElements, GL_UNSIGNED_INT, 0));
			GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		}
		else {
			GL_CALL(glDrawElements(type, nElements, GL_UNSIGNED_INT, 0));
		}
	
	}
}