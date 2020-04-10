#pragma once
#include "pch.h"
#include "Config.h"
#include "../OpenGLContext.h"
namespace gx {
	enum GXTexture2DType {//TODO move this from here
		GX_NONE=0,
		GX_DIFFUSE,
		GX_SPECULAR,
		GX_SPECULAR_MAP,
		GX_NORMAL,
		GX_HEIGHT,
		GX_AMBIENT
	};
	enum GXTexture2DColorType {//TODO move this from here
		GX_RGBA=GL_RGBA,
		GX_RGB=GL_RGB,
		GX_RED=GL_RED
	};
	enum GXTexture2DFilteringMethod {//TODO move this from here
		GX_NEAREST = GL_NEAREST,
		GX_LINEAR = GL_LINEAR

	};

	class GX_DLL GLTexture2D
	{
	public:
		inline void init(GXuint32 id, GXTexture2DType Type) {
			ID = id;
			type = Type;
		}
		void init(const uint8_t* data, GXint32 width, GXint32 height, GXTexture2DColorType colorType, GXTexture2DType Type, GXTexture2DFilteringMethod method);
		inline static void setActiveTexture(GXuint32 pos) { glActiveTexture(GL_TEXTURE0 + pos); }
		inline void use() {
			glBindTexture(GL_TEXTURE_2D,ID);
		}
		inline static void use(GXuint32 id) {
			glBindTexture(GL_TEXTURE_2D, id);
		}
		inline static void stop() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		inline static void destroy(GXuint32 id) {

			glDeleteTextures(1, &id);
		}
		inline GXuint32 getID()const { return ID; }
		inline GXTexture2DType getType() const { return type; }
	private:
		GXuint32 ID;
		GXTexture2DType type;
	};
}
