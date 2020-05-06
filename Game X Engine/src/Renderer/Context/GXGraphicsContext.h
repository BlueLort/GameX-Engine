#pragma once
#include "Config.h"
#include "Renderer/Enums.h"
#ifdef USING_OPENGL
	#include "Renderer/API/openGL/OpenGLContext.h"
#endif

namespace gx {
#ifdef USING_OPENGL
	class GX_DLL GXGraphicsContext
	{
	public:
		inline static void init() {
			OpenGLContext::init();
		}
		inline static SDL_GLContext getGLContext() { return OpenGLContext::getGLContext(); }
		inline static const GXuint8* getVendorName() {
			return OpenGLContext::getVendorName();
		}
		inline static const GXuint8* getVersion() {
			return OpenGLContext::getGLVersion();
		}
		inline static const GXuint8* getGPUName() {
			return OpenGLContext::getGPUName();
		}
		inline static void setViewPort(GXuint32 Width, GXuint32 Height) {
			OpenGLContext::setViewPort(Width, Height);
		}
		inline static void enableFlag(GXRendererFlag flag) {
			OpenGLContext::enableFlag(flag);
		}
		inline static void disableFlag(GXRendererFlag flag) {
			OpenGLContext::disableFlag(flag);
		}
		inline static void setCullFace(GXCullFace cullingFace) {
			OpenGLContext::setCullFace(cullingFace);
		}
		inline static void clearBufferBits(GXint32 bits) {
			OpenGLContext::clearBufferBits(bits);
		}
		inline static void setClearColor(GXFloat r, GXFloat g, GXFloat b, GXFloat a) {
			OpenGLContext::setClearColor(r, g, b, a);
		}
		inline static void setLineWidth(GXFloat width) {
			OpenGLContext::setLineWidth(width);
		}
		inline static void setReadAttachment(GXint32 idx) {
			OpenGLContext::setReadAttachment(idx);
		}
		inline static void readPixel(GXint32 locX, GXint32 locY, GXint32 width, GXint32 height, GXint32 internalFormat, GXint32 type, void* storage) {
			OpenGLContext::readPixel(locX, locY, width, height, internalFormat, type, storage);
		}
		inline static void destroy() {
			OpenGLContext::destroy();
		}
	};
#endif
}


