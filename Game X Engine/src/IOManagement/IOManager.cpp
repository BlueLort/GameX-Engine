#include "pch.h"
#include "IOManager.h"
static std::mutex IMAGE_READ_GAURD;
namespace gx {
	namespace io {

		std::unordered_map<std::string, GXuint32> IOManager::texIDs;
		std::unordered_map<std::string, std::vector<std::shared_ptr<GXMeshComponent>>> IOManager::modelsImported;
		std::unordered_map<std::string, std::string> IOManager::textImported;
		std::vector <std::future<void>> IOManager::asyncTasks;
		std::queue< std::pair<std::string, std::vector< std::shared_ptr<MeshData> > > > IOManager::meshesNeedToBeProcessed;
		std::queue< std::shared_ptr<ImageData> > IOManager::texturesNeedToBeProcessed;
		std::string IOManager::readFile(const char* filePath)
		{
			std::fstream fin(filePath, std::ios::in);
			std::string out = "";
			if (fin.is_open()) {
				std::string line;
				while (std::getline(fin, line))
				{
					out += line;
				}
				GXE_DEBUG("Text File imported successfully\nPath: {0}", filePath);
				textImported[std::string(filePath)] = out;
				return out;
			}
			GXE_ERROR("Failed to import text file\nPath: {0}", filePath);
			return out;
		}
		std::shared_ptr<ImageData> IOManager::imageRead(const char* filePath, GXTexture2DType Type, bool async)
		{
			std::lock_guard<std::mutex> locker(IMAGE_READ_GAURD);
			std::shared_ptr<ImageData> iData;
			iData.reset(new ImageData());
			iData->data = stbi_load(filePath, &iData->width, &iData->height, &iData->nChannels, 0);
			iData->type = Type;
			strcpy_s(iData->filePath, filePath);
			if (async)texturesNeedToBeProcessed.emplace(iData);
			return iData;
		}
		std::shared_ptr<GXTexture2D> IOManager::readTexture(std::shared_ptr<ImageData>& iData) {
			std::shared_ptr<GXTexture2D> tex;
			tex.reset(new GXTexture2D());
			std::string fPath(iData->filePath);
			if (texIDs[fPath] != 0) {
				tex->init(texIDs[fPath], iData->type);
				return tex;
			}
			if (iData->data) {
				GXTexture2DColorType textype = (iData->nChannels == 4 ? GX_RGBA : GX_RGB);
				tex->init(iData->data, iData->width, iData->height, textype, textype, iData->type, GXTexture2DFilteringMethod::GX_LINEAR, GX_UNSIGNED_BYTE);
				GXE_DEBUG("Texture2D imported successfully\nPath: {0}", iData->filePath);
				texIDs[fPath] = tex->getID();
			}
			else {
				GXE_ERROR("Failed to import Texture2D\nPath: {0}", iData->filePath);
			}

			//Data Freed on destruction using the destructor of ImageData
			return tex;
		}

		void IOManager::importModel(const char* filePath, const char* fileName)
		{
			std::string sFilePath(filePath);
			std::string sFileName(fileName);
			auto ite = modelsImported.find(sFilePath + sFileName);
			if (ite != modelsImported.end()) {
				return;
			}
			asyncTasks.emplace_back(std::async(std::launch::async, assimpRead, sFilePath, sFileName));
		}



		//make a copy of the string to avoid garbage on threading
		void IOManager::assimpRead(std::string filePath, std::string fileName) {
			//most of the logic can be found in learnopengl.com
			Assimp::Importer importer;
			std::string file(filePath);
			file += fileName;
			const aiScene* scene = importer.ReadFile(file.c_str(),
				aiProcess_OptimizeGraph
				| aiProcess_OptimizeMeshes
				| aiProcess_Triangulate
				| aiProcess_FlipUVs
				| aiProcess_GenNormals
				| aiProcess_CalcTangentSpace

			);
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				GXE_ERROR("Failed to load Assimp model..\nPath: {0}\n{1}", filePath, importer.GetErrorString());

				return;
			}
			meshesNeedToBeProcessed.emplace(assimpProcessNode(filePath.c_str(), fileName.c_str(), scene->mRootNode, scene));
		}




		std::pair<std::string, std::vector< std::shared_ptr<MeshData> > > IOManager::assimpProcessNode(const char* filePath, const char* fileName, aiNode* node, const aiScene* scene)
		{
			std::vector<std::shared_ptr<MeshData>> currentNodeMeshes;
			std::vector< std::future< std::pair<std::string, std::vector< std::shared_ptr<MeshData>> > > > loadedMeshes;
			for (GXuint32 i = 0; i < node->mNumChildren; i++)
			{
				loadedMeshes.emplace_back(std::async(std::launch::async, assimpProcessNode, filePath, fileName, node->mChildren[i], scene));
			}
			for (GXuint32 i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				currentNodeMeshes.emplace_back(assimpProcessMesh(filePath, fileName, mesh, scene));
			}
			for (GXint32 i = 0; i < loadedMeshes.size(); i++) {
				std::vector<std::shared_ptr<MeshData>> childNodeMeshes = loadedMeshes[i].get().second;
				currentNodeMeshes.insert(currentNodeMeshes.end(), childNodeMeshes.begin(), childNodeMeshes.end());
			}
			std::string file = std::string(filePath);
			file += fileName;
			return { file ,currentNodeMeshes };
		}


		std::shared_ptr<MeshData> IOManager::assimpProcessMesh(const char* filePath, const char* fileName, aiMesh* mesh, const aiScene* scene)
		{
			std::shared_ptr<MeshData> mData;
			mData.reset(new MeshData());

			//Vertices
			for (GXuint32 i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex3D vert;
				vert.position.x = mesh->mVertices[i].x;
				vert.position.y = mesh->mVertices[i].y;
				vert.position.z = mesh->mVertices[i].z;
				vert.normal.x = mesh->mNormals[i].x;
				vert.normal.y = mesh->mNormals[i].y;
				vert.normal.z = mesh->mNormals[i].z;
				if (mesh->mTextureCoords[0])
				{
					vert.texCoords.x = mesh->mTextureCoords[0][i].x;
					vert.texCoords.y = mesh->mTextureCoords[0][i].y;
				}
				else {
					vert.texCoords = GXVec2(0.0f, 0.0f);
				}
				vert.tangent.x = mesh->mTangents[i].x;
				vert.tangent.y = mesh->mTangents[i].y;
				vert.tangent.z = mesh->mTangents[i].z;

				vert.bitangent.x = mesh->mBitangents[i].x;
				vert.bitangent.y = mesh->mBitangents[i].y;
				vert.bitangent.z = mesh->mBitangents[i].z;
				mData->verts.emplace_back(vert);
			}
			//indices
			for (GXuint32 i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (GXuint32 j = 0; j < face.mNumIndices; j++)
					mData->indices.emplace_back(face.mIndices[j]);
			}


			//Materials/Textures
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			//diffuse
			std::vector<std::shared_ptr<ImageData>> diffuse = assimpImportTextures2D(filePath, material, aiTextureType_DIFFUSE, GXTexture2DType::GX_DIFFUSE);
			mData->texturesData.insert(mData->texturesData.end(), diffuse.begin(), diffuse.end());
			//specular
			std::vector<std::shared_ptr<ImageData>> specular = assimpImportTextures2D(filePath, material, aiTextureType_SPECULAR, GXTexture2DType::GX_SPECULAR);
			mData->texturesData.insert(mData->texturesData.end(), specular.begin(), specular.end());
			//normal
			std::vector<std::shared_ptr<ImageData>> normal = assimpImportTextures2D(filePath, material, aiTextureType_HEIGHT, GXTexture2DType::GX_NORMAL);
			mData->texturesData.insert(mData->texturesData.end(), normal.begin(), normal.end());
			//height
			std::vector<std::shared_ptr<ImageData>> height = assimpImportTextures2D(filePath, material, aiTextureType_AMBIENT, GXTexture2DType::GX_HEIGHT);
			mData->texturesData.insert(mData->texturesData.end(), height.begin(), height.end());
			return mData;
		}

		std::vector<std::shared_ptr<ImageData>> IOManager::assimpImportTextures2D(const char* filePath, aiMaterial* mat, aiTextureType type, GXTexture2DType gxTexType)
		{
			std::vector<std::shared_ptr<ImageData>> textures;
			for (GXuint32 i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString fileName;
				mat->GetTexture(type, i, &fileName);
				aiString file(filePath);
				file.Append(fileName.C_Str());
				textures.push_back(imageRead(file.C_Str(), gxTexType, false));
			}
			return textures;
		}



		std::shared_ptr<GXGraphicsBufferManager> IOManager::createBufferLayout(std::vector<Vertex3D>& verts, std::vector<GXuint32>& indices, std::vector<std::shared_ptr<GXTexture2D>>& textures)
		{
			std::shared_ptr<GXGraphicsBufferManager> Buffer;
			Buffer.reset(new GXGraphicsBufferManager());
			Buffer->initFull(&verts[0], sizeof(Vertex3D) * verts.size(), sizeof(Vertex3D));
			Buffer->uploadIndicesToBuffer(&indices[0], indices.size() * sizeof(GXuint32), indices.size());
			Buffer->setAttribPointer(0, 3, GX_FLOAT, offsetof(Vertex3D, position));
			Buffer->setAttribPointer(1, 3, GX_FLOAT, offsetof(Vertex3D, normal));
			Buffer->setAttribPointer(2, 2, GX_FLOAT, offsetof(Vertex3D, texCoords));
			Buffer->setAttribPointer(3, 3, GX_FLOAT, offsetof(Vertex3D, tangent));
			Buffer->setAttribPointer(4, 3, GX_FLOAT, offsetof(Vertex3D, bitangent));
			Buffer->endStream();
			for (GXint32 i = 0; i < textures.size(); i++) {
				Buffer->addTexture(textures[i]);
			}
			return Buffer;

		}
		void IOManager::update()
		{
			if (!meshesNeedToBeProcessed.empty()) {
				auto meshes = meshesNeedToBeProcessed.front();
				meshesNeedToBeProcessed.pop();
				std::vector<std::shared_ptr<GXMeshComponent>> components;
				for (auto& mesh : meshes.second) {
					std::vector<std::shared_ptr<GXTexture2D>> textures;
					for (auto& tex : mesh->texturesData) {
						textures.emplace_back(readTexture(tex));
					}
					//TODO HANDLE GXID initializing to this component
					components.emplace_back(new GXMeshComponent(mesh->ownerID, createBufferLayout(mesh->verts, mesh->indices, textures), std::make_shared<GXPickingCollider>(mesh->verts)));
				}

				GXE_INFO("Model has been imported successfully ,File Path: {0} ", meshes.first);
				modelsImported[meshes.first] = components;


			}

			if (!texturesNeedToBeProcessed.empty()) {
				auto tex = texturesNeedToBeProcessed.front();
				texturesNeedToBeProcessed.pop();
				readTexture(tex);
			}

		}
		void IOManager::destroy()
		{
			destroyModels();
			destroyTextures();
			while (!asyncTasks.empty()) {
				asyncTasks.pop_back();
			}
		}

		inline void IOManager::destroyModels() {
			for (auto& ite : modelsImported) {
				for (auto& meshComp : ite.second) {
					meshComp->destroy();
				}
			}

		}
		inline void IOManager::destroyTextures() {
			for (auto& ite : texIDs) {
				GXTexture2D::destroy(ite.second);
			}
		}
	}
}