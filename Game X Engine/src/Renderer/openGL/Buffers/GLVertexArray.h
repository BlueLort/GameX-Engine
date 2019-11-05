#pragma once
#include "Config.h"
#include "../OpenGLContext.h"
namespace gx {

	class GX_DLL GLVertexArray {
	private:
		GLuint ID;
		GLuint stride;
		void init(uint32_t StrideLength);
		inline void use() {
			glBindVertexArray(ID);
		}

		inline static void stop() {
			glBindVertexArray(0);
		}

		//Assumeing Vertex Array is bound
		void setAttribPointers(uint32_t ShaderLoc, uint32_t Count, GLenum Type, uint32_t offset);
		void destroy();
		friend class GLBufferManager;

	};

}