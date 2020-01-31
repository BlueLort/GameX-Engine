#pragma once
#include "pch.h"
#include "Config.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>
#include <stb_image/stb_image_write.h>
#include "Renderer/openGL/Buffers/GLBufferManager.h"
#include "Renderer/openGL/Texture/GLTexture2D.h"
#include "Maths/Data/Vertex3D.h"
#include "Components/Mesh/GXMeshComponent.h"
namespace gx {
	namespace io {
		struct GX_DLL ImageData {
			char filePath[256];
			uint8_t* data = nullptr;
			int32_t width = -1;
			int32_t height = -1;
			int32_t nChannels = -1;
			GXTexture2DType type = NONE;
			~ImageData() {
				stbi_image_free(data);
			}
		};
		struct GX_DLL MeshData {
			std::vector<Vertex3D> verts;
			std::vector<uint32_t> indices;
			std::vector<std::shared_ptr<ImageData>> texturesData;
		};
		class GX_DLL IOManager {
		public:
			IOManager() = delete;
			//TODO implement this to read custom shaders or any files.
			static const char* readFile(const char* filePath);

			static std::shared_ptr<ImageData> imageRead(const char* filePath, GXTexture2DType Type);
			//Cached GLTexture reading.
			static std::shared_ptr<GLTexture2D> GLReadTexture(std::shared_ptr<ImageData>& iData);

			//TODO make this return GXModelObject ptr instead.
			//opengl context is obstacle for multithreaded loading for now only multithread processing
			static std::vector<std::shared_ptr<GXMeshComponent>> importModel(const char* filePath, const char* fileName, GLShader* glshader);

			static void destroy();
		private:
			static std::vector<std::shared_ptr<MeshData>> assimpRead(const char* filePath, const char* fileName);
			static std::vector<std::shared_ptr<MeshData>> assimpProcessNode(const char* filePath, aiNode* node, const aiScene* scene);
			static std::shared_ptr<MeshData> assimpProcessMesh(const char* filePath, aiMesh* mesh, const aiScene* scene);
			
			static std::vector<std::shared_ptr<ImageData>> assimpImportTextures2D(const char* filePath, aiMaterial* mat, aiTextureType type, GXTexture2DType gxTexType);
			


			static std::shared_ptr<GLBufferManager> GLCreateBufferLayout(std::vector<Vertex3D>& verts, std::vector<uint32_t>& indices, std::vector<std::shared_ptr<GLTexture2D>>& textures);

			static std::unordered_map<std::string, GLuint> texIDs;
			static std::unordered_map<std::string, std::vector<std::shared_ptr<GXMeshComponent>>> modelsImported;

			static void destroyGLModels();
			static void destroyTextures();
		};
	}
}