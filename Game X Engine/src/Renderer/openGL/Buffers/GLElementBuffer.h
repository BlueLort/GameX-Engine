#pragma once
#include "Config.h"
#include "../OpenGLContext.h"
namespace gx {
	class GX_DLL GLElementBuffer {
	private:
		GXuint32 ID;
		void init();
		inline void use() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		}


		inline static void stop() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		}


		//Assumeing Element Buffer is bound
		void uploadData(GXuint32* data, GXuint32 size);
		inline static void destroy(GXuint32 id) {
			GL_CALL(glDeleteBuffers(1, &id));
		}
		friend class GLBufferManager;
	};
}