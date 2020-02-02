#include "pch.h"
#include "IOManager.h"
static std::mutex IMAGE_READ_GAURD;
namespace gx {
	namespace io {
	
		std::unordered_map<std::string, GLuint> IOManager::texIDs;
		std::unordered_map<std::string, std::vector<std::shared_ptr<GXMeshComponent>>> IOManager::modelsImported;
		std::vector <std::future<void>> IOManager::asyncTasks;
		std::queue< std::pair<std::string, std::vector< std::shared_ptr<MeshData> > > > IOManager::meshesNeedToBeProcessed;
		std::queue< std::shared_ptr<ImageData> > IOManager::texturesNeedToBeProcessed;
		const char* IOManager::readFile(const char* filePath)
		{
			return nullptr;
		}
		std::shared_ptr<ImageData> IOManager::imageRead(const char* filePath, GXTexture2DType Type,bool async)
		{
			std::lock_guard<std::mutex> locker(IMAGE_READ_GAURD);
			std::shared_ptr<ImageData> iData;
			iData.reset(new ImageData());
			iData->data=stbi_load(filePath, &iData->width, &iData->height, &iData->nChannels, 0);
			iData->type = Type;
			strcpy_s(iData->filePath,filePath);
			if(async)texturesNeedToBeProcessed.emplace(iData);
			return iData;
		}
		std::shared_ptr<GLTexture2D> IOManager::GLReadTexture(std::shared_ptr<ImageData>& iData) {
			std::shared_ptr<GLTexture2D> tex;
			tex.reset(new GLTexture2D());
			std::string fPath(iData->filePath);
			if (texIDs[fPath] != 0) {
				tex->init(texIDs[fPath], iData->type);
				return tex;
			}
			if (iData->data) {
				tex->init(iData->data, iData->width,iData->height,iData->nChannels==4?GX_RGBA:GX_RGB,iData->type);
				GXE_DEBUG("GLTexture2D imported successfully\nPath: {0}", iData->filePath);
				texIDs[fPath] = tex->getID();
			}
			else {
				GXE_ERROR("Failed to import GLTexture2D\nPath: {0}", iData->filePath);
			}
			
			//Data Freed on destruction using the destructor of ImageData
			return tex;
		}
		
		void IOManager::importModel(const char* filePath, const char* fileName )
		{
			std::string sFilePath = std::string(filePath);
			auto ite = modelsImported.find(sFilePath);
			if (ite != modelsImported.end()) {
				return;
			} 
			asyncTasks.emplace_back(std::async(std::launch::async, assimpRead, filePath, fileName));
		}

		
		
		void IOManager::assimpRead(const char* filePath,const char* fileName) {
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
			meshesNeedToBeProcessed.emplace(assimpProcessNode(filePath, scene->mRootNode, scene));
		}

		
		
		
		std::pair<std::string, std::vector< std::shared_ptr<MeshData> > > IOManager::assimpProcessNode(const char* filePath, aiNode* node, const aiScene* scene)
		{
			std::vector<std::shared_ptr<MeshData>> currentNodeMeshes;
			std::vector< std::future< std::pair<std::string, std::vector< std::shared_ptr<MeshData>> > > > loadedMeshes;
			for (uint32_t i = 0; i < node->mNumChildren; i++)
			{
				loadedMeshes.emplace_back(std::async(std::launch::async,assimpProcessNode,filePath,node->mChildren[i], scene));
			}
			for (uint32_t i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				currentNodeMeshes.emplace_back(assimpProcessMesh(filePath, mesh, scene));
			}
			for (int32_t i = 0; i < loadedMeshes.size(); i++) {
				std::vector<std::shared_ptr<MeshData>> childNodeMeshes = loadedMeshes[i].get().second;
				currentNodeMeshes.insert(currentNodeMeshes.end(),childNodeMeshes.begin(), childNodeMeshes.end());
			}
			return {std::string(filePath) ,currentNodeMeshes };
		}
		
		
		std::shared_ptr<MeshData> IOManager::assimpProcessMesh(const char* filePath, aiMesh* mesh, const aiScene* scene)
		{
			std::shared_ptr<MeshData> mData;
			mData.reset(new MeshData());
			
			//Vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
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
			for (uint32_t i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (uint32_t j = 0; j < face.mNumIndices; j++)
					mData->indices.emplace_back(face.mIndices[j]);
			}

			
			//Materials/Textures
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			//diffuse
			std::vector<std::shared_ptr<ImageData>> diffuse = assimpImportTextures2D(filePath,material, aiTextureType_DIFFUSE,GXTexture2DType::GX_DIFFUSE);
			mData->texturesData.insert(mData->texturesData.end(), diffuse.begin(), diffuse.end());
			//specular
			std::vector<std::shared_ptr<ImageData>> specular = assimpImportTextures2D(filePath,material, aiTextureType_SPECULAR,  GXTexture2DType::GX_SPECULAR);
			mData->texturesData.insert(mData->texturesData.end(), specular.begin(), specular.end());
			//normal
			std::vector<std::shared_ptr<ImageData>> normal = assimpImportTextures2D(filePath,material, aiTextureType_HEIGHT,  GXTexture2DType::GX_NORMAL);
			mData->texturesData.insert(mData->texturesData.end(), normal.begin(), normal.end());
			//height
			std::vector<std::shared_ptr<ImageData>> height = assimpImportTextures2D(filePath,material, aiTextureType_AMBIENT,  GXTexture2DType::GX_HEIGHT);
			mData->texturesData.insert(mData->texturesData.end(), height.begin(), height.end());
			return mData;
		}
		
		std::vector<std::shared_ptr<ImageData>> IOManager::assimpImportTextures2D(const char* filePath, aiMaterial* mat, aiTextureType type, GXTexture2DType gxTexType)
		{
			std::vector<std::shared_ptr<ImageData>> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString fileName;
				mat->GetTexture(type, i, &fileName);
				aiString file(filePath);
				file.Append(fileName.C_Str());
				textures.push_back(imageRead(file.C_Str(), gxTexType,false));
			}
			return textures;
		}

	
		
		std::shared_ptr<GLBufferManager> IOManager::GLCreateBufferLayout(std::vector<Vertex3D>& verts, std::vector<uint32_t>& indices, std::vector<std::shared_ptr<GLTexture2D>>& textures)
		{
			std::shared_ptr<GLBufferManager> Buffer;
			Buffer.reset(new GLBufferManager());
			Buffer->initFull(&verts[0], sizeof(Vertex3D) * verts.size(), sizeof(Vertex3D));
			Buffer->uploadIndicesToBuffer(&indices[0], indices.size() * sizeof(uint32_t),indices.size());
			Buffer->setAttribPointer(0, 3, GL_FLOAT, offsetof(Vertex3D, position));
			Buffer->setAttribPointer(1, 3, GL_FLOAT, offsetof(Vertex3D, normal));
			Buffer->setAttribPointer(2, 2, GL_FLOAT, offsetof(Vertex3D, texCoords));
			Buffer->setAttribPointer(3, 3, GL_FLOAT, offsetof(Vertex3D, tangent));
			Buffer->setAttribPointer(4, 3, GL_FLOAT, offsetof(Vertex3D, bitangent));
			//TODO Add Tangents bitangents later
			Buffer->endStream();
			for(int i=0;i<textures.size();i++){
				Buffer->addTexture(textures[i]);
			}
			verts.clear();
			return Buffer;
		}
		void IOManager::update()
		{
			if (!meshesNeedToBeProcessed.empty()) {
				auto meshes = meshesNeedToBeProcessed.front();
				meshesNeedToBeProcessed.pop();
				std::vector<std::shared_ptr<GXMeshComponent>> components;
				for (auto mesh : meshes.second) {
					std::vector<std::shared_ptr<GLTexture2D>> textures;
					for (auto tex : mesh->texturesData) {
						textures.emplace_back(GLReadTexture(tex));
					}
					components.emplace_back(new GXMeshComponent(GLCreateBufferLayout(mesh->verts, mesh->indices, textures)));
				}

				GXE_INFO("Model has been imported successfully ,File Path: {0} ", meshes.first);

				modelsImported[meshes.first] = components;
			}

			if (!texturesNeedToBeProcessed.empty()) {
				auto tex = texturesNeedToBeProcessed.front();
				texturesNeedToBeProcessed.pop();
				GLReadTexture(tex);
			}

		}
		void IOManager::destroy()
		{
			destroyGLModels();
			destroyTextures();
		}

		inline void IOManager::destroyGLModels() {
			for (auto ite : modelsImported) {
				for (auto meshComp : ite.second) {
					meshComp->destroy();
				}
			}

		}
		inline void IOManager::destroyTextures() {
			for (auto ite : texIDs) {
				GLTexture2D::destroy(ite.second);
			}
		}
	}
}