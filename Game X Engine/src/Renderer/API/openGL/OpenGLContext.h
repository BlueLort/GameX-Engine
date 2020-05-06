#pragma once
#include "Config.h"
#include "GLLoader/glad.h"
#include "Window/GXWindow.h"
#include "Renderer/Enums.h"
namespace gx {
	class GX_DLL OpenGLContext
	{
	public:
		static void init();
		inline static SDL_GLContext getGLContext() { return glContext; }
		inline static const GXuint8* getVendorName() { return glGetString(GL_VENDOR); }
		inline static const GXuint8* getGLVersion() { return glGetString(GL_VERSION); }
		inline static const GXuint8* getGPUName() { return glGetString(GL_RENDERER); }
		inline static void setViewPort(GXuint32 Width, GXuint32 Height) { glViewport(0, 0, Width, Height); }
		inline static void enableFlag(GXRendererFlag flag) {glEnable(flag);}
		inline static void disableFlag(GXRendererFlag flag) {glDisable(flag);}
		inline static void setCullFace(GXCullFace cullingFace) { glCullFace(cullingFace); }
		inline static void clearBufferBits(GXint32 bits) {glClear(bits);}
		inline static void setClearColor(GXFloat r,GXFloat g,GXFloat b,GXFloat a ) {glClearColor(r,g,b,a);}
		inline static void setLineWidth(GXFloat width){ glLineWidth(width); }
		inline static void setReadAttachment(GXint32 idx) {glReadBuffer(idx);}
		inline static void readPixel(GXint32 locX, GXint32 locY, GXint32 width, GXint32 height,GXint32 internalFormat,GXint32 type ,void* storage) {
			GL_CALL(glReadPixels(locX, locY, width, height,internalFormat,type, storage));
		}
		inline static void destroy() {SDL_GL_DeleteContext(glContext);}
	private:
		static SDL_GLContext glContext;
	};

}
