#pragma once
#include "Config.h"
#include "../Texture/GLTexture2D.h"
#include "../Window/GXWindow.h"
namespace gx {
	constexpr GXuint32 GX_ATTACHMENTS = 5;
	enum GXAttachmentType {
		GX_COLOR_ATTACHMENT = 0, // Albedo/Color Attachment
		GX_DEPTH_ATTACHMENT,
		GX_POSITION_ATTACHMENT,
		GX_NORMAL_ATTACHMENT,
		GX_ID_ATTACHMENT,
		GX_STENCIL_ATTACHMENT

	};
	enum GXFBOBindMethod {
		GX_FBO_READ = GL_READ_FRAMEBUFFER,
		GX_FBO_WRITE = GL_DRAW_FRAMEBUFFER,
		GX_FBO_RW = GL_FRAMEBUFFER

	};
	class GX_DLL GLFrameBuffer {
	public:

		//init frame buffer with defaul RBO
		inline void init(GXint32 width, GXint32 height) {
			this->width = width;
			this->height = height;
			glGenFramebuffers(1, &ID);
			glBindFramebuffer(GL_FRAMEBUFFER, ID);

			//Render Buffer Object
			glGenRenderbuffers(1, &RBO);
			glBindRenderbuffer(GL_RENDERBUFFER, RBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				GXE_ERROR("Error Creating FBO !");
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);


		}
		inline void use(GXFBOBindMethod method) {
			glBindFramebuffer(method, ID);
		}
		inline void useTexture(GXAttachmentType type) {
			glBindTexture(GL_TEXTURE_2D, textureAttachments[type]);
		}
		inline static void stop() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		inline void destroy() {
			GL_CALL(glDeleteFramebuffers(1, &ID));
		}

		inline GXuint32 getTextureID(GXAttachmentType type)const { return textureAttachments[type]; }
		inline GXint32 getWidth() const { return width; }
		inline GXint32 getHeight() const { return height; }
		inline GXuint32 getAttachmentIndex(GXAttachmentType type) const { return GL_COLOR_ATTACHMENT0 + type; }
		inline void setDrawBuffeers(GXuint32 total,GXuint32* types) {
			glBindFramebuffer(GL_FRAMEBUFFER, ID);
			GL_CALL(glDrawBuffers(total , types));
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		}
		inline void addTextureAttachment(GXAttachmentType type) {
			glBindFramebuffer(GL_FRAMEBUFFER, ID);
			//Texture Init.
			glGenTextures(1, &textureAttachments[type]);
			glBindTexture(GL_TEXTURE_2D, textureAttachments[type]);
			//TODO Width and height to the actuall rendering window size along with glviewport
			switch (type)
			{
			case gx::GX_COLOR_ATTACHMENT:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + type, GL_TEXTURE_2D, textureAttachments[type], 0);
				break;
			case gx::GX_DEPTH_ATTACHMENT:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureAttachments[type], 0);
				break;
			case gx::GX_POSITION_ATTACHMENT:case gx::GX_NORMAL_ATTACHMENT:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + type, GL_TEXTURE_2D, textureAttachments[type], 0);
				break;
			case gx::GX_ID_ATTACHMENT:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + type, GL_TEXTURE_2D, textureAttachments[type], 0);
				break;
			case gx::GX_STENCIL_ATTACHMENT:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureAttachments[type], 0);
				break;


			default:
				break;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		

		}
	private:
		GXuint32 ID;
		GXuint32 textureAttachments[GX_ATTACHMENTS];
		GXuint32 RBO;
		GXint32 width, height;
	};
}