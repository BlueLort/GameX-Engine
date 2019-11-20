#pragma once
#include "pch.h"
#include "Config.h"
#include "../OpenGLContext.h"
namespace gx {
	enum GLTexture2DType {
		NONE=0,
		DIFFUSE,
		SPECULAR,
		SPECULAR_MAP,
		NORMAL,
		HEIGHT,
		AMBIENT
	};
	class GX_DLL GLTexture2D
	{
	public:
		inline void init(GLuint id, GLTexture2DType Type) {
			ID = id;
			type = Type;
		}
		void init(const uint8_t* data, int32_t width, int32_t height, GLTexture2DType Type);
		inline void use() {
			glBindTexture(GL_TEXTURE_2D,ID);
		}
		inline static void stop() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		inline void destroy() {

			glDeleteTextures(1, &ID);
		}
		inline GLuint getID()const { return ID; }
	private:
		GLuint ID;
		GLTexture2DType type;
	};
}
