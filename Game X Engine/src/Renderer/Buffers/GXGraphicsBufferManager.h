#pragma once

#include "pch.h"
#include "Config.h"
#include "Renderer/Texture/GXTexture2D.h"
#ifdef USING_OPENGL
	#include "Renderer/API/openGL/Buffers/GLBufferManager.h"
#endif


namespace gx {
#ifdef USING_OPENGL
	class GX_DLL GXGraphicsBufferManager {
	public:
		GXGraphicsBufferManager() {

		}
		inline void init(GXuint32 requiredSize, GXuint32 strideSize) {
			glBufferManager.init(requiredSize, strideSize);
		}
		inline void initFull(void* data, GXuint32 requiredSize, GXuint32 strideSize) {
			glBufferManager.initFull(data,requiredSize,strideSize);
		}
		inline void uploadDataToBuffer(void* data, GXuint32 size) {
			glBufferManager.uploadDataToBuffer(data, size);
		}
		inline void setAttribPointer(GXuint32 loc, GXuint32 count, GXEnumType type, GXuint32 offset) {
			glBufferManager.setAttribPointer(loc,count,type,offset);
		}
		inline void uploadIndicesToBuffer(GXuint32* indices, GXuint32 size, GXuint32 length) {
			glBufferManager.uploadIndicesToBuffer(indices, size, length);
		}
		inline void addTexture(const std::shared_ptr<GXTexture2D>& tex) {
			glBufferManager.addTexture(tex);
		}
		inline void bindDynamicBuffer() { glBufferManager.bindDynamicBuffer(); }
		inline void unbindDynamicBuffer() { glBufferManager.bindDynamicBuffer(); }
		inline void endStream() {
			glBufferManager.endStream();
		}
		inline void use() {
			glBufferManager.use();
		}
		inline static void stop() {
			GLBufferManager::stop();
		}
		inline void updateDynamicBuffer(void* data, GXuint32 size) {
			glBufferManager.updateDynamicBuffer(data, size);
		}
		inline void setAttribIPointer(GXuint32 loc, GXuint32 count, GXEnumType type, GXuint32 offset) {
			glBufferManager.setAttribIPointer(loc, count, type, offset);
		}
		inline void setAttribPointer(GXuint32 loc, GXuint32 count, GXEnumType type, GXuint32 offset, GXuint32 stride) {
			glBufferManager.setAttribPointer(loc, count, type, offset, stride);
		}
		inline void setAttribIPointer(GXuint32 loc, GXuint32 count, GXEnumType type, GXuint32 offset, GXuint32 stride) {
			glBufferManager.setAttribIPointer(loc, count, type, offset, stride);
		}
		inline void setVertexAttribDivisor(GXuint32 loc, GXuint32 divisor) {
			glBufferManager.setVertexAttribDivisor(loc, divisor);
		}
		inline std::vector<std::shared_ptr<GXTexture2D>> getTextures()const { return glBufferManager.getTextures(); }
		inline GXuint32 getNumberOfElements()const { return glBufferManager.getNumberOfElements(); }
		inline void destroy() {
			glBufferManager.destroy();
		}
	private:
		GLBufferManager glBufferManager;
	};
#endif

	
}