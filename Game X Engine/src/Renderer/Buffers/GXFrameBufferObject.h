#pragma once
#include "Config.h"
#ifdef USING_OPENGL
#include "Renderer/API/openGL/Buffers/GLFrameBufferObject.h"
#endif
namespace gx {


#ifdef USING_OPENGL

	class GX_DLL GXFrameBuffer {
	public:
		//init frame buffer with defaul RBO
		inline void init(GXint32 width, GXint32 height,GXBool initDepthStencilRBO = true) {
			glFBO.init(width, height, initDepthStencilRBO);
		}
		inline void use(GXFBOBindMethod method) {
			glFBO.use(method);
		}
		inline void useTexture(GXAttachmentType type) {
			glFBO.useTexture(type);
		}
		inline static void stop() {
			GLFrameBuffer::stop();
		}
		inline void destroy() {
			glFBO.destroy();
		}
		inline GXuint32 getTextureID(GXAttachmentType type)const { return glFBO.getTextureID(type); }
		inline GXint32 getWidth() const { return glFBO.getWidth(); }
		inline GXint32 getHeight() const { return glFBO.getHeight(); }
		inline GXuint32 getAttachmentIndex(GXAttachmentType type) const { return glFBO.getAttachmentIndex(type); }
		inline void setDrawBuffeers(GXuint32 total, GXuint32* types) { glFBO.setDrawBuffeers(total, types); }
		inline void addTextureAttachment(GXAttachmentType type) {glFBO.addTextureAttachment(type);}
	private:
		GLFrameBuffer glFBO;
	};
#endif
}