#include "pch.h"
#include "GXQuad.h"

namespace gx {
	GXuint32 GXQuad::currentID = 0;
	void GXQuad::draw(const GXShader* glshader)
	{
		graphicsBufferManager->use();
		GLRenderer::getInstance().draw(graphicsBufferManager->getNumberOfElements(), RenderType::GX_TRIANGLES, false);
		graphicsBufferManager->stop();
	}

	void GXQuad::destroy()
	{
		graphicsBufferManager->destroy();
	}

}