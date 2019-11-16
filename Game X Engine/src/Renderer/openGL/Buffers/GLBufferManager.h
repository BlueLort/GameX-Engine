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
		void init(const std::string& ID,uint32_t requiredSize,uint32_t strideLength);
		void init(uint32_t requiredSize, uint32_t strideLength);
		void initFull(const std::string& ID,void* data, uint32_t requiredSize, uint32_t strideLength);
		void initFull(void* data,uint32_t requiredSize, uint32_t strideLength);
		void uploadDataToBuffer(void* data, uint32_t size);
		void setAttribPointer(uint32_t loc,uint32_t count, GLenum type, uint32_t offset);
		void uploadIndicesToBuffer(uint32_t* indices, uint32_t size);
		inline void addTexture(const std::shared_ptr<GLTexture2D>& tex) {
			textures.emplace_back(tex);
		}
		//Delete the VBO and EBO and VAO ptrs
		void endStream();
		inline void bind() {
			GLVertexArray::use(VAOID);
		}
		static void destroy();
	private:
		static std::map<std::string,GLuint> storedData;
		GLuint VAOID;//the only thing needed to render
		std::unique_ptr<GLVertexArray> VAO;
		std::unique_ptr <GLVertexBuffer> VBO;
		std::unique_ptr <GLElementBuffer> EBO;
		std::vector<std::shared_ptr<GLTexture2D>> textures;



	};
}