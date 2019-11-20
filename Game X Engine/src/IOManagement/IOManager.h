#pragma once
#include "pch.h"
#include "Config.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>
#include "Renderer/openGL/Buffers/GLBufferManager.h"
#include "Renderer/openGL/Texture/GLTexture2D.h"
#include "Maths/Data/Vertex3D.h"
namespace gx {
	namespace io {
		class GX_DLL IOManager {
		public:
			IOManager() = delete;
			//TODO implement this to read custom shaders or any files.
			static const char* readFile(const char* filePath);
			static std::shared_ptr<GLTexture2D> GLimageRead(const char* filePath, GLTexture2DType Type);

			static void GLAssimpRead(const char* filePath, const char* fileName, std::vector<std::shared_ptr<GLBufferManager>>& meshDataArr);
		private:
			static void GLProcessNode(const char* filePath, aiNode* node, const aiScene* scene,std::unordered_map<std::string, std::shared_ptr<GLTexture2D>>& materialsLoaded,std::vector<std::shared_ptr<GLBufferManager>>& meshDataArr);
			static std::shared_ptr<GLBufferManager> GLProcessMesh(const char* filePath, aiMesh* mesh, const aiScene* scene, std::unordered_map<std::string, std::shared_ptr<GLTexture2D>>& materialsLoaded);
			static std::vector<std::shared_ptr<GLTexture2D>> GLImportTextures2D(const char* filePath, aiMaterial* mat, aiTextureType type, std::unordered_map<std::string, std::shared_ptr<GLTexture2D>>& materialsLoaded, GLTexture2DType glTexType);
			static std::shared_ptr<GLBufferManager> GLCreateBufferLayout(std::vector<Vertex3D>& verts, std::vector<uint32_t>& indices,std::vector<std::shared_ptr<GLTexture2D>>& textures);

			static std::unordered_map<std::string, GLuint> texIDs;
		};
	}
}