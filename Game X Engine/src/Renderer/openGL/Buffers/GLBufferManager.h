#pragma once

#include "pch.h"
#include "Config.h"

#include "GLElementBuffer.h"
#include "GLVertexArray.h"
#include "GLVertexBuffer.h"
#include "../Texture/GLTexture2D.h"

namespace gx {
	class GX_DLL GLBufferManager {
	public:
		GLBufferManager() {

		}
		void init(uint32_t requiredSize, uint32_t strideSize);
		void initFull(void* data,uint32_t requiredSize, uint32_t strideSize);
		void uploadDataToBuffer(void* data, uint32_t size);
		void setAttribPointer(uint32_t loc,uint32_t count, GLenum type, uint32_t offset);
		void uploadIndicesToBuffer(uint32_t* indices, uint32_t size,uint32_t length);
		inline void addTexture(const std::shared_ptr<GLTexture2D>& tex) {
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
		inline std::vector<std::shared_ptr<GLTexture2D>> getTextures()const { return textures; }
		inline uint32_t getNumberOfElements()const { return nElements; }
		inline void destroy() {
			GLVertexArray::destroy(VAOID);
			//GLVertexBuffer::destroy(VBOID);//deleted on endstream.
			GLElementBuffer::destroy(EBOID);
		}
	private:
		GLuint VAOID,VBOID,EBOID;//the only thing needed to render
		uint32_t nElements;
		std::unique_ptr<GLVertexArray> VAO;
		std::unique_ptr <GLVertexBuffer> VBO;
		std::unique_ptr <GLElementBuffer> EBO;
		std::vector<std::shared_ptr<GLTexture2D>> textures;



	};
}