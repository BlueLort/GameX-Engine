#pragma once
#include "Config.h"
#include "../OpenGLContext.h"
namespace gx {

	class GX_DLL GLVertexArray {
	private:
		GLuint ID;
		GLuint stride;
		void init(uint32_t StrideLength);
		inline static void use(GLuint id) {
			glBindVertexArray(id);
		}

		inline static void stop() {
			glBindVertexArray(0);
		}
		inline static void destroy(GLuint id) {
			GL_CALL(glDeleteVertexArrays(1, &id));
		}
		//Assumeing Vertex Array is bound
		void setAttribPointers(uint32_t ShaderLoc, uint32_t Count, GLenum Type, uint32_t offset);
		
		friend class GLBufferManager;

	};

}