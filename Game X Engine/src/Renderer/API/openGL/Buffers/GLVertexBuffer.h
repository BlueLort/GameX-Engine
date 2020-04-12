#pragma once

#include "Config.h"
#include "../OpenGLContext.h"

namespace gx {

	class GX_DLL GLVertexBuffer {

	private:
		GXuint32 ID;
		GXuint32 currentSize;
		GXuint32 maxSize;
		void init(GXuint32 requiredSize);
		void initFull(void* data,GXuint32 requiredSize);
		inline void use() {
			glBindBuffer(GL_ARRAY_BUFFER, this->ID);
		}

		inline static void stop() {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		//Buffer Must be Bound before uploading data !
		void uploadData(void* data,GXuint32 size);
		inline static void destroy(GXuint32 id) {
			GL_CALL(glDeleteBuffers(1, &id));
		}
		friend class GLBufferManager;
	};

}