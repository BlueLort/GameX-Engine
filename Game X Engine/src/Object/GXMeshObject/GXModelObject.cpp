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


	}

}