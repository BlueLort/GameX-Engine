#pragma once
#include "pch.h"
#include "Config.h"
#include "Renderer/Enums.h"
#ifdef USING_OPENGL
	#include "Renderer/API/openGL/Texture/GLTexture2D.h"
#endif
namespace gx {
#ifdef USING_OPENGL

	class GX_DLL GXTexture2D
	{
	public:
		inline void init(GXuint32 id, GXTexture2DType Type) {
			gltexture.init(id, Type);
		}
		inline void init(const void* data, GXint32 width, GXint32 height, gx::GXTexture2DColorType colorTypeInternal, gx::GXTexture2DColorType colorType, gx::GXTexture2DType Type, gx::GXTexture2DFilteringMethod method, gx::GXEnumType datatype) {
			gltexture.init(data, width, height, colorTypeInternal, colorType, Type, method, datatype);
		}
		inline static void setActiveTexture(GXuint32 pos) { GLTexture2D::setActiveTexture(pos); }
		inline void use() {
			
			gltexture.use();
		}
		inline static void use(GXuint32 id) {
			GLTexture2D::use(id);
		}
		inline static void stop() {
			GLTexture2D::stop();
		}
		inline static void destroy(GXuint32 id) {
			GLTexture2D::destroy(id);
		}
		inline GXuint32 getID()const { return gltexture.getID(); }
		inline GXTexture2DType getType() const { return gltexture.getType(); }
	private:
		GLTexture2D gltexture;
	};
#endif
}
