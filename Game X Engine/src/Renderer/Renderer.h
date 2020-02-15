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
	enum DepthFunc {
#ifdef USING_OPENGL
		GX_NEVER = GL_NEVER,
		GX_LESS = GL_LESS,
		GX_EQUAL = GL_EQUAL,
		GX_LEQUAL = GL_LEQUAL,
		GX_GREATER = GL_GREATER,
		GX_GEQUAL = GL_GEQUAL,
		GX_NOTEQUAL = GL_NOTEQUAL,
		GX_ALWAYS = GL_ALWAYS
#endif
	};
	enum WindingOrder {
#ifdef USING_OPENGL
		GX_CCW = GL_CCW,
		GX_CW = GL_CW,
#endif
	};
	class GX_DLL Renderer {
	public:
		virtual void init()=0;
		virtual void draw(uint32_t nElements,RenderType type, bool isWireFrame) = 0;
		virtual inline void setViewPort(uint32_t Width, uint32_t Height) = 0;
		virtual inline void setDepthFunc(DepthFunc func) = 0;
		virtual inline void setWindingOrder(WindingOrder type) = 0;
	};
}