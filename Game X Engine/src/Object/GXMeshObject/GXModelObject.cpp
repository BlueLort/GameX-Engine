#include "pch.h"
#include "GXModelObject.h"

namespace gx {

	void GXModelObject::GLinit(const char* fileName, const char* shaderPath) {
	
		if (shaderPath == nullptr) {
			glshader = GLShaderManager::getShader(GLShaderType::DEFAULT_GBUFFER);
		}
		else {
			glshader = GLShaderManager::getShader(shaderPath);
		}

		io::IORequestHandler::getModel(fileName, &components, &isReady);
#ifdef USING_OPENGL
		//set constant data
		this->glshader->use();
		this->glshader->setFloat("material.shininess", 32.0f);
		this->glshader->setUInt("objID", this->GXID);
		GLShader::stop();
#endif		

	}

	void GXModelObject::update(float deltaTime) {
		//For rendering as rendering happens in GXMeshComponent
#ifdef USING_OPENGL
		this->glshader->use();
		SceneLightManager::getInstance().setLightValues(this->glshader);
		this->glshader->setMat4("model", transform.getModel());
		this->glshader->setMat4("vp", EditorCamera::getInstance().getPVMatrix());
#endif
		for (auto& component : components) {
			component->update(deltaTime);
			component->draw(glshader,isWireFrame);
		}
	}

	void GXModelObject::destroy() {
		for (GXint32 i = 0; i < components.size(); i++) {
			components[i]->destroy();
		}
	}

}