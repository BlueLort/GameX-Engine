#include "pch.h"
#include "IOManager.h"


namespace gx {
	namespace io {
		std::unordered_map<std::string, GLuint> IOManager::texIDs;
		//std::unordered_map<std::string, std::shared_ptr<GLBufferManager>> IOManager::GLModelBuffers;
		const char* IOManager::readFile(const char* filePath)
		{
			return nullptr;
		}
		std::shared_ptr<GLTexture2D> IOManager::GLimageRead(const char* filePath, GLTexture2DType Type) {
			std::shared_ptr<GLTexture2D> tex;
			tex.reset(new GLTexture2D());
			std::string fPath(filePath);
			if (texIDs[fPath] != 0) {
				tex->init(texIDs[fPath], Type);
				return tex;
			}
			int32_t width, height, nChannels;
			uint8_t* data= stbi_load(filePath, &width, &height, &nChannels, 0);
			if (data) {
				tex->init(data, width, height, Type);
				GXE_DEBUG("Texture imported successfully\nPath: {0}", filePath);
				texIDs[fPath] = tex->getID();
			}
			else {
				GXE_ERROR("Failed to import texture\nPath: {0}", filePath);
			}
			

			stbi_image_free(data);
			return tex;
		}
		
		void IOManager::GLAssimpRead(const char* filePath,const char* fileName, std::vector<std::shared_ptr<GXComponent>>& components,GLShader* glshader) {
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

			std::unordered_map<std::string, std::shared_ptr<GLTexture2D>> materialsLoaded;
			GLProcessNode(filePath,scene->mRootNode, scene,materialsLoaded,components,glshader);
			GXE_INFO("Model has been imported successfully\nName: {0}\nPath: {1}",fileName, filePath);
		}

		void IOManager::destroy()
		{
			destroyGLModels();
			destroyTextures();
		}

		

		void IOManager::GLProcessNode(const char* filePath, aiNode* node, const aiScene* scene, std::unordered_map<std::string, std::shared_ptr<GLTexture2D>>& materialsLoaded, std::vector<std::shared_ptr<GXComponent>>& components, GLShader* glshader)
		{
			for (uint32_t i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				std::shared_ptr<gx::GLBufferManager> GLBM = GLProcessMesh(filePath, mesh, scene, materialsLoaded);
				components.emplace_back(std::make_shared<GXMeshComponent>(GLBM, glshader));
			}
			for (uint32_t i = 0; i < node->mNumChildren; i++)
			{
				GLProcessNode(filePath,node->mChildren[i], scene,materialsLoaded,components,glshader);
			}
			
		}
		std::shared_ptr<GLBufferManager> IOManager::GLProcessMesh(const char* filePath, aiMesh* mesh, const aiScene* scene, std::unordered_map<std::string, std::shared_ptr<GLTexture2D>>& materialsLoaded)
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
				indices.reserve(face.mNumIndices);
				for (uint32_t j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}

			
			//Materials/Textures
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			//diffuse
			std::vector<std::shared_ptr<GLTexture2D>> diffuse = GLImportTextures2D(filePath,material, aiTextureType_DIFFUSE,materialsLoaded,GLTexture2DType::DIFFUSE);
			textures.insert(textures.end(), diffuse.begin(), diffuse.end());
			//specular
			std::vector<std::shared_ptr<GLTexture2D>> specular = GLImportTextures2D(filePath,material, aiTextureType_SPECULAR, materialsLoaded, GLTexture2DType::SPECULAR);
			textures.insert(textures.end(), specular.begin(), specular.end());
			//normal
			std::vector<std::shared_ptr<GLTexture2D>> normal = GLImportTextures2D(filePath,material, aiTextureType_HEIGHT, materialsLoaded, GLTexture2DType::NORMAL);
			textures.insert(textures.end(), normal.begin(), normal.end());
			//height
			std::vector<std::shared_ptr<GLTexture2D>> height = GLImportTextures2D(filePath,material, aiTextureType_AMBIENT, materialsLoaded, GLTexture2DType::HEIGHT);
			textures.insert(textures.end(), height.begin(), height.end());
			return GLCreateBufferLayout(verts, indices, textures);
		}
		std::vector<std::shared_ptr<GLTexture2D>> IOManager::GLImportTextures2D(const char* filePath, aiMaterial* mat, aiTextureType type, std::unordered_map<std::string, std::shared_ptr<GLTexture2D>>& materialsLoaded, GLTexture2DType glTexType)
		{
			std::vector<std::shared_ptr<GLTexture2D>> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString fileName;
				mat->GetTexture(type, i, &fileName);
				aiString file(filePath);
				file.Append(fileName.C_Str());
				std::string mkey(fileName.C_Str());
				if (materialsLoaded[mkey] != nullptr) {
					textures.push_back(materialsLoaded[mkey]);
				}
				else {
					std::shared_ptr<GLTexture2D> Texture = GLimageRead(file.C_Str(), glTexType);
					textures.push_back(Texture);
					materialsLoaded[fileName.C_Str()] = Texture;
				}
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
		inline void IOManager::destroyGLModels() {
			//for (auto ite : GLModelBuffers) {
			//	ite.second->destroy();
			//	}

		}
		inline void IOManager::destroyTextures() {
			for (auto ite : texIDs) {
				GLTexture2D::destroy(ite.second);
			}
		}
	}
}