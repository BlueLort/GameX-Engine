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
		void init(const uint8_t* data, int32_t width, int32_t height,bool hasAlpha, GLTexture2DType Type);
		inline static void setActiveTexture(uint32_t pos) { glActiveTexture(GL_TEXTURE0 + pos); }
		inline void use() {
			glBindTexture(GL_TEXTURE_2D,ID);
		}
		inline static void stop() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		inline static void destroy(GLuint id) {

			glDeleteTextures(1, &id);
		}
		inline GLuint getID()const { return ID; }
		inline GLTexture2DType getType() const { return type; }
	private:
		GLuint ID;
		GLTexture2DType type;
	};
}
