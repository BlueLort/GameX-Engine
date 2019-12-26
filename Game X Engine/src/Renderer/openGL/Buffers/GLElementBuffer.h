#pragma once
#include "Config.h"
#include "../OpenGLContext.h"
namespace gx {
	class GX_DLL GLElementBuffer {
	private:
		GLuint ID;
		void init();
		inline void use() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		}


		inline static void stop() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		}


		//Assumeing Element Buffer is bound
		void uploadData(uint32_t* data, uint32_t size);
		inline static void destroy(GLuint id) {
			GL_CALL(glDeleteBuffers(1, &id));
		}
		friend class GLBufferManager;
	};
}