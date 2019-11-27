#pragma once

#include "Config.h"
#include "openGL/OpenGLContext.h"

namespace gx {
	enum RenderType {
#ifdef USING_OPENGL
		GX_TRIANGLES = GL_TRIANGLES,
		GX_TRIANGLES_STRIP = GL_TRIANGLE_STRIP,
		GX_TRIANGLES_FAN = GL_TRIANGLE_FAN,
		GX_TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
		GX_TRIANGLES_STRIP_ADJACENCY=GL_TRIANGLES_ADJACENCY
#endif
	};
	class GX_DLL Renderer {
	public:
		virtual void init()=0;
		virtual void begin()=0;
		virtual void draw(uint32_t nElements,RenderType type) = 0;
		virtual void end()=0;
		virtual inline void setViewPort(uint32_t Width, uint32_t Height) = 0;
	};
}