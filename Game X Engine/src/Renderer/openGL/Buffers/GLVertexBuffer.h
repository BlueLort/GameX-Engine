#pragma once

#include "Config.h"
#include "../OpenGLContext.h"

namespace gx {

	class GX_DLL GLVertexBuffer {

	private:
		GLuint ID;
		uint32_t currentSize;
		uint32_t maxSize;
		void init(uint32_t requiredSize);
		void initFull(void* data,uint32_t requiredSize);
		inline void use() {
			glBindBuffer(GL_ARRAY_BUFFER, this->ID);
		}

		inline static void stop() {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		//Buffer Must be Bound before uploading data !
		void uploadData(void* data,uint32_t size);
		inline static void destroy(GLuint id) {
			GL_CALL(glDeleteBuffers(1, &id));
		}
		friend class GLBufferManager;
	};

}