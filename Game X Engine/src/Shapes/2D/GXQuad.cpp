#include "pch.h"
#include "GXQuad.h"

namespace gx {
	GXuint32 GXQuad::currentID = 0;
	void GXQuad::update(float deltaTime,const GLShader* glshader)
	{
		GLBM->use();
		GLRenderer::getInstance().draw(GLBM->getNumberOfElements(), RenderType::GX_TRIANGLES, false);
		GLBM->stop();
	}

	void GXQuad::destroy()
	{
		GLBM->destroy();
	}

}