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
			GXTexture2DType type = GX_NONE;
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
		
		private:
			//TODO implement this to read custom shaders or any files.
			static const char* readFile(const char* filePath);

			static std::shared_ptr<ImageData> imageRead(const char* filePath, GXTexture2DType Type, bool addToGLTextures);
			//Cached GLTexture reading.
			static std::shared_ptr<GLTexture2D> GLReadTexture(std::shared_ptr<ImageData>& iData);
			static void importModel(const char* filePath, const char* fileName);
			static void finishAllTasks() { for (int i = 0; i < asyncTasks.size(); i++)asyncTasks[i].get(); }
			static void update();
			static void destroy();

			static void assimpRead(std::string filePath, std::string fileName);
			static std::pair<std::string, std::vector< std::shared_ptr<MeshData> > > assimpProcessNode(const char* filePath, const char* fileName, aiNode* node, const aiScene* scene);
			static std::shared_ptr<MeshData> assimpProcessMesh(const char* filePath, const char* fileName, aiMesh* mesh, const aiScene* scene);
			static std::vector<std::shared_ptr<ImageData>> assimpImportTextures2D(const char* filePath, aiMaterial* mat, aiTextureType type, GXTexture2DType gxTexType);
			static std::shared_ptr<GLBufferManager> GLCreateBufferLayout(std::vector<Vertex3D>& verts, std::vector<uint32_t>& indices, std::vector<std::shared_ptr<GLTexture2D>>& textures);
			static void destroyGLModels();
			static void destroyTextures();

			static std::unordered_map<std::string, GLuint> texIDs;
			static std::unordered_map<std::string, std::vector<std::shared_ptr<GXMeshComponent>>> modelsImported;

			static std::vector <std::future<void>> asyncTasks;
			static std::queue< std::pair<std::string,std::vector< std::shared_ptr<MeshData> > > > meshesNeedToBeProcessed;
			static std::queue< std::shared_ptr<ImageData> > texturesNeedToBeProcessed;

			friend class IORequestHandler;

		};
	}
}