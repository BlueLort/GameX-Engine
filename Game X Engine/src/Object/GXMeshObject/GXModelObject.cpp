#include "pch.h"
#include "GXModelObject.h"

namespace gx {




	void GXModelObject::init(const char* filePath, const char* shaderPath) {
	
		//TODO SOLVE AND READ THE MODEL
		//io::IOManager::GLAssimpRead(filePath, meshRenderIDs);
		if (shaderPath == nullptr) {
	#ifdef  USING_OPENGL
			shader=GLShaderManager::getShader(GLShaderType::DEFAULT);
		}
		else {
			shader=GLShaderManager::getShader(shaderPath);
	#endif
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

}