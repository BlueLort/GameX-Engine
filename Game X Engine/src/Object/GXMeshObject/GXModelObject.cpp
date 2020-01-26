#include "pch.h"
#include "GXModelObject.h"

namespace gx {

	void GXModelObject::GLinit(const char* filePath, const char* fileName , const char* shaderPath) {
	
		if (shaderPath == nullptr) {
			glshader = GLShaderManager::getShader(GLShaderType::DEFAULT_LIGHT);
		}
		else {
			glshader = GLShaderManager::getShader(shaderPath);
		}
		std::vector<std::shared_ptr<GXMeshComponent>> comps=io::IOManager::importModel(filePath,fileName,glshader);
		for (auto comp : comps) {
			addComponent(comp);
		}
		
		
	}

	void GXModelObject::update(float deltaTime) {
		//For rendering as rendering happens in GXMeshComponent
		this->glshader->use();
		SceneLightManager::getInstance().setLightValues(this->glshader);
		GXMat4 model = GXMat4(1.0f);
		this->glshader->setMat4("model", model);
		this->glshader->setMat4("vp", EditorCamera::getInstance().getPVMatrix());
		this->glshader->setFloat("material.shininess", 32.0f);
		for (auto component : components) {
			component->update(deltaTime);
		}
	}

	void GXModelObject::destroy() {
		for (int i = 0; i < components.size(); i++) {
			components[i]->destroy();
		}
	}

}