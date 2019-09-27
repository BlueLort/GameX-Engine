#pragma once

#include "Config.h"
#include "openGL/OpenGLContext.h"

namespace gx {

	class GX_DLL Renderer {
	public:
		static void init();
		static void begin();
		static void end();
		static void setViewPort(uint32_t Width, uint32_t Height);


	};
	inline void Renderer::setViewPort(uint32_t Width, uint32_t Height)
	{
#ifdef USING_OPENGL
		gx::OpenGLContext::setViewPort(Width, Height);
#endif 

	}
}