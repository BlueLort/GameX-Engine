#pragma once

#include "Config.h"
#include "Enums.h"
#include "Renderer/Context/GXGraphicsContext.h"


namespace gx {
	class GX_DLL Renderer {
	public:
		virtual void init()=0;
		virtual void draw(GXuint32 nElements,RenderType type, bool isWireFrame) = 0;
		virtual inline void setViewPort(GXuint32 Width, GXuint32 Height) = 0;
		virtual inline void setDepthFunc(DepthFunc func) = 0;
		virtual inline void setWindingOrder(WindingOrder type) = 0;
	};
}
