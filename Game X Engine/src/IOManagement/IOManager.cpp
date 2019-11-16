#include "pch.h"
#include "IOManager.h"


namespace gx {
	namespace io {
		std::unordered_map<const char*, GLuint> IOManager::texIDs;
		const char* IOManager::readFile(const char* filePath)
		{
			return nullptr;
		}
		std::shared_ptr<GLTexture2D> IOManager::GLimageRead(const char* filePath, GLTexture2DType Type) {
			std::shared_ptr<GLTexture2D> tex;
			tex.reset(new GLTexture2D());
			if (texIDs[filePath] != 0) {
				tex->init(texIDs[filePath], Type);
				return tex;
			}
			int32_t width, height, nChannels;
			uint8_t* data= stbi_load(filePath, &width, &height, &nChannels, 0);
			if (data) {
				tex->init(data, width, height, Type);
				GXE_INFO("Texture imported successfully,Path: {0}", filePath);
			}
			else {
				GXE_ERROR("Failed to import texture,Path: {0}", filePath);
			}

			stbi_image_free(data);
			return tex;
		}
		
		void IOManager::GLAssimpRead(const char* filePath, std::vector<std::shared_ptr<GLBufferManager>>& meshDataArr) {
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(filePath,
				aiProcess_OptimizeGraph
				| aiProcess_OptimizeMeshes
				| aiProcess_Triangulate
				| aiProcess_FlipUVs
				| aiProcess_GenNormals
				| aiProcess_CalcTangentSpace
			);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				GXE_ERROR("Failed to load Assimp Model..\nPath: {0}\n{1}", filePath, importer.GetErrorString());
				return;
			}
			std::unordered_map<const char*, std::shared_ptr<GLTexture2D>> materialsLoaded;
			GLProcessNode(scene->mRootNode, scene,materialsLoaded,meshDataArr);
			GXE_INFO("Model Read Successfully,Path: {0}", filePath);
		}

		

		void IOManager::GLProcessNode(aiNode* node, const aiScene* scene, std::unordered_map<const char*, std::shared_ptr<GLTexture2D>>& materialsLoaded, std::vector<std::shared_ptr<GLBufferManager>>& meshDataArr)
		{
			for (uint32_t i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshDataArr.push_back(GLProcessMesh(mesh, scene,materialsLoaded));
			}
			for (uint32_t i = 0; i < node->mNumChildren; i++)
			{
				GLProcessNode(node->mChildren[i], scene,materialsLoaded,meshDataArr);
			}
			
		}
		std::shared_ptr<GLBufferManager> IOManager::GLProcessMesh(aiMesh* mesh, const aiScene* scene, std::unordered_map<const char*, std::shared_ptr<GLTexture2D>>& materialsLoaded)
		{
			std::vector<Vertex3D> verts;
			std::vector<uint32_t> indices;
			std::vector<std::shared_ptr<GLTexture2D>> textures;
			
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
					glm::vec2 vec;
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
				verts.emplace_back(vert);
			}
			//indices
			for (uint32_t i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (uint32_t j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}
			//Materials/Textures
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			//diffuse
			std::vector<std::shared_ptr<GLTexture2D>> diffuse = GLImportTextures2D(material, aiTextureType_DIFFUSE,materialsLoaded,GLTexture2DType::DIFFUSE);
			textures.insert(textures.end(), diffuse.begin(), diffuse.end());
			//specular
			std::vector<std::shared_ptr<GLTexture2D>> specular = GLImportTextures2D(material, aiTextureType_SPECULAR, materialsLoaded, GLTexture2DType::SPECULAR);
			textures.insert(textures.end(), specular.begin(), specular.end());
			//normal
			std::vector<std::shared_ptr<GLTexture2D>> normal = GLImportTextures2D(material, aiTextureType_HEIGHT, materialsLoaded, GLTexture2DType::NORMAL);
			textures.insert(textures.end(), normal.begin(), normal.end());
			//height
			std::vector<std::shared_ptr<GLTexture2D>> height = GLImportTextures2D(material, aiTextureType_AMBIENT, materialsLoaded, GLTexture2DType::HEIGHT);
			textures.insert(textures.end(), height.begin(), height.end());


			return GLCreateBufferLayout(verts, indices, textures);
		}
		std::vector<std::shared_ptr<GLTexture2D>> IOManager::GLImportTextures2D(aiMaterial* mat, aiTextureType type, std::unordered_map<const char*, std::shared_ptr<GLTexture2D>>& materialsLoaded, GLTexture2DType glTexType)
		{
			std::vector<std::shared_ptr<GLTexture2D>> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				auto ite = materialsLoaded.find(str.C_Str());
				if (ite != materialsLoaded.end()) {
					textures.push_back(ite->second);
				}
				else {
					std::shared_ptr<GLTexture2D> Texture = GLimageRead(str.C_Str(), glTexType);
					textures.push_back(Texture);
					materialsLoaded[str.C_Str()] = Texture;
				}
			}
			return textures;
		}
		std::shared_ptr<GLBufferManager> IOManager::GLCreateBufferLayout(std::vector<Vertex3D>& verts, std::vector<uint32_t>& indices, std::vector<std::shared_ptr<GLTexture2D>>& textures)
		{
			std::shared_ptr<GLBufferManager> Buffer;
			Buffer.reset(new GLBufferManager);
			Buffer->initFull(reinterpret_cast<void*>(&verts[0]), sizeof(Vertex3D) * verts.size(), sizeof(Vertex3D));
			Buffer->uploadIndicesToBuffer(&indices[0], indices.size() * sizeof(uint32_t));
			Buffer->setAttribPointer(0, 3, GL_FLOAT, offsetof(Vertex3D, position));
			Buffer->setAttribPointer(1, 3, GL_FLOAT, offsetof(Vertex3D, normal));
			Buffer->setAttribPointer(2, 2, GL_FLOAT, offsetof(Vertex3D, texCoords));
			//TODO Add Tangents bitangents later
			Buffer->endStream();
			for(int i=0;i<textures.size();i++){
				Buffer->addTexture(textures[i]);
			}
			return Buffer;
		}
	}
}