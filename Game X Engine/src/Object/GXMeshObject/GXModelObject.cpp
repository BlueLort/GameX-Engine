#include "pch.h"
#include "GXModelObject.h"

namespace gx {

	void GXModelObject::GLinit(const char* filePath, const char* fileName , const char* shaderPath) {
	
		//TODO SOLVE AND READ THE MODEL
		io::IOManager::GLAssimpRead(filePath,fileName, meshRenderIDs);
		if (shaderPath == nullptr) {
			glshader=GLShaderManager::getShader(GLShaderType::DEFAULT);
		}
		else {
			glshader=GLShaderManager::getShader(shaderPath);
		}
	}

	void GXModelObject::update(float deltaTime) {
		for (auto component : components) {
			component->update(deltaTime);
		}
	}

	void GXModelObject::destroy() {
		for (int i = 0; i < meshRenderIDs.size(); i++) {
			meshRenderIDs[i]->destroy();
		}
	}

	void GXModelObject::GLDraw(GLShader* sh)
	{



	}
	void GXModelObject::GLDraw()
	{
		this->glshader->use();
		for (auto ite : meshRenderIDs) {
			uint32_t nDiffuse = 0;
			uint32_t nSpecular = 0;
			auto textures = ite->getTextures();
			for (uint32_t i = 0; i < textures.size(); i++)
			{
				GLTexture2D::setActiveTexture(i);
				GLTexture2DType type = textures[i]->getType();
				std::string name;
				switch (type)
				{
				case gx::DIFFUSE:
					name="material.diffuse"+std::to_string(nDiffuse++);
					break;
				case gx::SPECULAR:
					name = "material.specular" + std::to_string(nSpecular++);
					break;
				case gx::SPECULAR_MAP:
					break;
				case gx::NORMAL:
					break;
				case gx::HEIGHT:
					break;
				case gx::AMBIENT:
					break;
				default:

					break;
				}
				this->glshader->setFloat(name.c_str(), i);
				textures[i]->use();
			}
			GLTexture2D::setActiveTexture(0);

			// draw mesh
			ite->use();
			GLRenderer::getInstance().draw(ite->getNumberOfElements(), RenderType::GX_TRIANGLES);
			GLBufferManager::stop();
		}
		

	}

}