#include "pch.h"
#include "GXModelObject.h"

namespace gx {

	void GXModelObject::init(const char* fileName, const char* shaderPath) {
	
		if (shaderPath == nullptr) {
			shader = GXShaderManager::getShader(GXCompiledShader::DEFAULT_GBUFFER);
		}
		else {
			shader = GXShaderManager::getShader(shaderPath);
		}

		io::IORequestHandler::getModel(fileName, &components, &isReady);	

	}

	void GXModelObject::update(float deltaTime) {
		for (auto& component : components) {
			component->update(deltaTime);
		}
	}

	void GXModelObject::draw()
	{
		this->shader->use();
		SceneLightManager::getInstance().setLightValues(this->shader);
		this->shader->setMat4("model", transform.getModel());
		this->shader->setMat4("vp", EditorCamera::getInstance().getPVMatrix());
		this->shader->setFloat("material.shininess", 32.0f);
		this->shader->setUInt("objID", this->GXID);
		for (auto& component : components) {
			component->draw(shader, RenderType::GX_TRIANGLES, isWireFrame);
		}
	}

	void GXModelObject::destroy() {
		for (GXint32 i = 0; i < components.size(); i++) {
			components[i]->destroy();
		}
	}

}