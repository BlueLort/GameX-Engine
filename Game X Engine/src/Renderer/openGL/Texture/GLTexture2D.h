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
		inline void use() {
			glBindTexture(GL_TEXTURE_2D,ID);
		}
		inline void init(GLuint id, GLTexture2DType Type) {
			ID = id;
			type = Type;
		}
		inline void init(const uint8_t* data,int32_t width,int32_t height,GLTexture2DType Type){
		
			glGenTextures(1, &ID);
			glBindTexture(GL_TEXTURE_2D, ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		inline static void stop() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		inline void destroy() {

			glDeleteTextures(1, &ID);
		}

	private:
		GLuint ID;
		GLTexture2DType type;
	};
}
