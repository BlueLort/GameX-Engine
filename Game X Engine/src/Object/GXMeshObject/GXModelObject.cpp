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
		std::shared_ptr<GXPhysicsComponent> gxp = std::make_shared<GXPhysicsComponent>(GXID);
		gxp->setOwnerTransform(&transform);
		components.push_back(gxp);

	}

	void GXModelObject::update(float deltaTime) {
		//For rendering as rendering happens in GXMeshComponent
#ifdef USING_OPENGL
		this->glshader->use();
		SceneLightManager::getInstance().setLightValues(this->glshader);
		this->glshader->setMat4("model", transform.getModel());
		this->glshader->setMat4("vp", EditorCamera::getInstance().getPVMatrix());
		this->glshader->setFloat("material.shininess", 32.0f);
#endif
		for (auto& component : components) {
			component->update(deltaTime);
			component->draw(glshader,isWireFrame);
		}
	}

	void GXModelObject::destroy() {
		for (int i = 0; i < components.size(); i++) {
			components[i]->destroy();
		}
	}

}