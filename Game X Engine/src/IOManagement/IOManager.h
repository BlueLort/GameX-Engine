#pragma once
#include "pch.h"
#include "Config.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>
#include <stb_image/stb_image_write.h>
#include "Renderer/Buffers/GXGraphicsBufferManager.h"
#include "Renderer/Texture/GXTexture2D.h"
#include "Maths/Data/Vertex3D.h"
#include "Components/Mesh/GXMeshComponent.h"
#include "Physics/GXPickingCollider.h"

namespace gx {
	namespace io {
		struct GX_DLL ImageData {
			char filePath[256];
			GXuint8* data = nullptr;
			GXint32 width = -1;
			GXint32 height = -1;
			GXint32 nChannels = -1;
			GXTexture2DType type = GX_NONE;
			~ImageData() {
				stbi_image_free(data);
			}
		};
		struct GX_DLL MeshData {
			std::vector<Vertex3D> verts;
			std::vector<GXuint32> indices;
			std::vector<std::shared_ptr<ImageData>> texturesData;
			GXuint32 ownerID;
		};
		
		class GX_DLL IOManager {
		public:
			IOManager() = delete;
		
		private:
			//TODO implement this to read custom shaders or any files.
			static std::string readFile(const char* filePath);

			static std::shared_ptr<ImageData> imageRead(const char* filePath, GXTexture2DType Type, bool addToTextures);
			//Cached Texture reading.
			static std::shared_ptr<GXTexture2D> readTexture(std::shared_ptr<ImageData>& iData);
			static void importModel(const char* filePath, const char* fileName);
			static void finishAllTasks() { for (GXint32 i = 0; i < asyncTasks.size(); i++)asyncTasks[i].get(); }
			static void update();
			static void destroy();

			static void assimpRead(std::string filePath, std::string fileName);
			static std::pair<std::string, std::vector< std::shared_ptr<MeshData> > > assimpProcessNode(const char* filePath, const char* fileName, aiNode* node, const aiScene* scene);
			static std::shared_ptr<MeshData> assimpProcessMesh(const char* filePath, const char* fileName, aiMesh* mesh, const aiScene* scene);
			static std::vector<std::shared_ptr<ImageData>> assimpImportTextures2D(const char* filePath, aiMaterial* mat, aiTextureType type, GXTexture2DType gxTexType);
			static std::shared_ptr<GXGraphicsBufferManager> createBufferLayout(std::vector<Vertex3D>& verts, std::vector<GXuint32>& indices, std::vector<std::shared_ptr<GXTexture2D>>& textures);
			static void destroyModels();
			static void destroyTextures();

			static std::unordered_map<std::string, GXuint32> texIDs;
			static std::unordered_map<std::string, std::vector<std::shared_ptr<GXMeshComponent>>> modelsImported;
			static std::unordered_map<std::string,std::string> textImported;

			static std::vector <std::future<void>> asyncTasks;
			static std::queue< std::pair<std::string,std::vector< std::shared_ptr<MeshData> > > > meshesNeedToBeProcessed;
			static std::queue< std::shared_ptr<ImageData> > texturesNeedToBeProcessed;

			friend class IORequestHandler;

		};
	}
}