#include "pch.h"
#include "GXModelObject.h"

namespace gx {

	void GXModelObject::GLinit(const char* filePath, const char* fileName , const char* shaderPath) {
	
		//TODO SOLVE AND READ THE MODEL
		io::IOManager::GLAssimpRead(filePath,fileName, meshRenderIDs);
		if (shaderPath == nullptr) {
			glshader=GLShaderManager::getShader(GLShaderType::DEFAULT_LIGHT);
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
		GXMat4 model = GXMat4(1.0f);
		this->glshader->setMat4("model", model);
		this->glshader->setMat4("vp",  EditorCamera::getInstance().getPVMatrix());
		this->glshader->setVec3("col",GXVec3(1.0f,0.5f,0.25f));
		std::shared_ptr<DirectionalLight> dLight = SceneLightManager::getInstance().getDirLight();
		std::shared_ptr<PointLight> pLight = SceneLightManager::getInstance().getPointLights()[0];
		std::shared_ptr<SpotLight> sLight = SceneLightManager::getInstance().getSpotLight();
		this->glshader->setVec3("material.diffuse", GXVec3(0.45f,0.3f,0.9f));
		this->glshader->setVec3("material.specular", GXVec3(0.2f, 0.2f, 0.2f));
		this->glshader->setFloat("material.shininess", 8.0f);

		this->glshader->setVec3("dirLight.direction", dLight->direction);
		this->glshader->setVec3("dirLight.ambient", dLight->ambientColor);
		this->glshader->setVec3("dirLight.diffuse", dLight->diffuseColor);
		this->glshader->setVec3("dirLight.specular", dLight->specularColor);
		// point light 1
		this->glshader->setVec3("pointLights[0].position", pLight->position);
		this->glshader->setVec3("pointLights[0].ambient", pLight->ambientColor);
		this->glshader->setVec3("pointLights[0].diffuse", pLight->diffuseColor);
		this->glshader->setVec3("pointLights[0].specular", pLight->specularColor);
		this->glshader->setFloat("pointLights[0].constant", pLight->constant);
		this->glshader->setFloat("pointLights[0].linear", pLight->linear);
		this->glshader->setFloat("pointLights[0].quadratic", pLight->quadratic);
		// spotLight
		this->glshader->setVec3("spotLight.position", sLight->position);
		this->glshader->setVec3("spotLight.direction", sLight->direction);
		this->glshader->setVec3("spotLight.ambient", sLight->ambientColor);
		this->glshader->setVec3("spotLight.diffuse", sLight->diffuseColor);
		this->glshader->setVec3("spotLight.specular", sLight->specularColor);
		this->glshader->setFloat("spotLight.constant", sLight->constant);
		this->glshader->setFloat("spotLight.linear", sLight->linear);
		this->glshader->setFloat("spotLight.quadratic", sLight->quadratic);
		this->glshader->setFloat("spotLight.cutOff", sLight->cutOff);
		this->glshader->setFloat("spotLight.outerCutOff", sLight->outerCutOff);
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
				// draw mesh
				
			}
			GLTexture2D::setActiveTexture(0);
			ite->use();
			GLRenderer::getInstance().draw(ite->getNumberOfElements(), RenderType::GX_TRIANGLES);
			GLBufferManager::stop();
		}
		

	}

}