#pragma once
#include "Config.h"
#include "Enums.h"
#ifdef USING_OPENGL
	#include "Renderer/API/openGL/GLRenderer.h"
#endif

namespace gx {
#ifdef USING_OPENGL
	class GX_DLL GXRenderer {
	public:
		inline static GXRenderer& getInstance() {
			static GXRenderer g;
			return g;
		}
		inline void init() {
			GLRenderer::getInstance().init();
		}
		inline void draw(GXuint32 nElements, RenderType type, bool isWireFrame) {
			GLRenderer::getInstance().draw(nElements, type, isWireFrame);
		}
		inline void setViewPort(GXuint32 Width, GXuint32 Height) {
			GLRenderer::getInstance().setViewPort(Width, Height);
		}
		inline void setDepthFunc(DepthFunc func) {
			GLRenderer::getInstance().setDepthFunc(func);
		}
		inline void setWindingOrder(WindingOrder type) {
			GLRenderer::getInstance().setWindingOrder(type);
		}
	};
#endif
}