#pragma once
#include "Config.h"
#include "../OpenGLContext.h"
namespace gx {

	class GX_DLL GLVertexArray {
	private:
		GXuint32 ID;
		GXuint32 stride;
		void init(GXuint32 StrideLength);
		inline static void use(GXuint32 id) {
			glBindVertexArray(id);
		}

		inline static void stop() {
			glBindVertexArray(0);
		}
		inline static void destroy(GXuint32 id) {
			GL_CALL(glDeleteVertexArrays(1, &id));
		}
		//Assumeing Vertex Array is bound
		void setAttribPointers(GXuint32 ShaderLoc, GXuint32 Count, GLenum Type, GXuint32 offset);
		
		friend class GLBufferManager;

	};

}