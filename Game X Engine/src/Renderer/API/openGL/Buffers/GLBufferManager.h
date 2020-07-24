#pragma once

#include "pch.h"
#include "Config.h"

#include "GLElementBuffer.h"
#include "GLVertexArray.h"
#include "GLVertexBuffer.h"
#include "Renderer/Texture/GXTexture2D.h"
#include "Renderer/Enums.h"
namespace gx {
	class GX_DLL GLBufferManager {
	public:
		GLBufferManager():VAOID(0), sVBOID(0), dVBOID(0), EBOID(0){

		}
		void init(GXuint32 requiredSize, GXuint32 strideSize);
		//init the full static data
		void initFull(void* data,GXuint32 requiredSize, GXuint32 strideSize);
		inline void bindDynamicBuffer() { dynamicVBO->use(); }
		inline void unbindDynamicBuffer() { GLVertexBuffer::stop(); }

		//update static data to the buffer
		void uploadDataToBuffer(void* data, GXuint32 size);
		void updateDynamicBuffer(void* data, GXuint32 size);
		void setAttribPointer(GXuint32 loc,GXuint32 count, GXEnumType type, GXuint32 offset);
		void setAttribIPointer(GXuint32 loc, GXuint32 count, GXEnumType type, GXuint32 offset);
		void setAttribPointer(GXuint32 loc, GXuint32 count, GXEnumType type, GXuint32 offset, GXuint32 stride);
		void setAttribIPointer(GXuint32 loc, GXuint32 count, GXEnumType type, GXuint32 offset, GXuint32 stride);
		void setVertexAttribDivisor(GXuint32 loc, GXuint32 divisor);
		void uploadIndicesToBuffer(GXuint32* indices, GXuint32 size,GXuint32 length);
		inline void addTexture(const std::shared_ptr<GXTexture2D>& tex) {
			textures.emplace_back(tex);
		}
		//Delete the VBO and EBO and VAO ptrs
		void endStream();
		inline void use() {
			GLVertexArray::use(VAOID);
		}
		inline static void stop() {
			GLVertexArray::stop();
		}
		inline std::vector<std::shared_ptr<GXTexture2D>> getTextures()const { return textures; }
		inline GXuint32 getNumberOfElements()const { return nElements; }
		inline void destroy() {
			GLVertexArray::destroy(VAOID);
			//GLVertexBuffer::destroy(VBOID);//deleted on endstream.
			GLElementBuffer::destroy(EBOID);
		}
	private:
		GXuint32 VAOID,sVBOID,dVBOID,EBOID;//the only thing needed to render
		GXuint32 nElements;
		std::unique_ptr<GLVertexArray> VAO;
		std::unique_ptr <GLVertexBuffer> staticVBO;
		std::unique_ptr <GLVertexBuffer> dynamicVBO;
		std::unique_ptr <GLElementBuffer> EBO;
		std::vector<std::shared_ptr<GXTexture2D>> textures;



	};
}