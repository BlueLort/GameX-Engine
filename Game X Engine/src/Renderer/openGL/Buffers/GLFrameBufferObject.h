#pragma once
#include "Config.h"
#include "../Texture/GLTexture2D.h"
#include "../Window/GXWindow.h"
namespace gx {
	class GX_DLL GLFrameBuffer {
	public:


		inline void use() {
			glBindFramebuffer(GL_FRAMEBUFFER, ID);
		}
		inline void useTexture() {
			glBindTexture(GL_TEXTURE_2D, textureAttachment);
		}
		inline static void stop() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		inline static void destroy(GLuint id) {
			GL_CALL(glDeleteFramebuffers(1, &id));
		}


		
	private:
		GLuint ID;
		GLuint textureAttachment;
		GLuint RBO;
		inline void init() {
			glGenFramebuffers(1, &ID);
			glBindFramebuffer(GL_FRAMEBUFFER, ID);
			//Texture Init.
			glGenTextures(1, &textureAttachment);
			glBindTexture(GL_TEXTURE_2D, textureAttachment);
			//TODO Width and height to the actuall rendering window size along with glviewport
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GXWindow::windowData->width, GXWindow::windowData->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureAttachment, 0);
			
			//Render Buffer Object
			glGenRenderbuffers(1, &RBO);
			glBindRenderbuffer(GL_RENDERBUFFER, RBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GXWindow::windowData->width, GXWindow::windowData->height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,RBO); 

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				GXE_ERROR("Error Creating FBO !");
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);


		}



	};
}