#pragma once
#include "pch.h"
#include "Config.h"
#include "Renderer/API/openGL/OpenGLContext.h"
#include "Renderer/Enums.h"
namespace gx {
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
