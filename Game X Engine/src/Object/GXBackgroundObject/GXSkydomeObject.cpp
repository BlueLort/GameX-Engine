#include "pch.h"
#include "GXSkydomeObject.h"

namespace gx {

	void GXSkydomeObject::init(const char* fileName, const char* shaderPath) {

		shader = GXShaderManager::getShader(GXCompiledShader::DEFAULT_SKYDOME);
		transform.scale = GXVec3(3000.0f, 3000.0f, 3000.0f);//enlarge the sphere
		staticModelMat4 = transform.getModel();
		//GET THE MODEL DATA
		io::IORequestHandler::getModel(fileName, &components, &isReady);
		objName = "Skydome";
	}

	void GXSkydomeObject::update(GXFloat deltaTime) {
		//For rendering as rendering happens in GXMeshComponent
		//First set the proper depth func to allow us draw skydome at the end of the frame
		GXRenderer::getInstance().setDepthFunc(GXCompareFunc::GX_LEQUAL);
		GXRenderer::getInstance().setWindingOrder(WindingOrder::GX_CW);//must be CW because we are inside it
		this->shader->use();
		this->shader->setMat4("model", staticModelMat4);
		this->shader->setMat4("view", EditorCamera::getInstance().getViewMatrix());
		this->shader->setMat4("projection", EditorCamera::getInstance().getProjectionMatrix());
		for (auto& component : components) {
			component->update(deltaTime);
			component->draw(shader,isWireFrame);
		}
		//reset values to default
		GXRenderer::getInstance().setDepthFunc(GXCompareFunc::GX_LESS);
		GXRenderer::getInstance().setWindingOrder(WindingOrder::GX_CCW);
	}

	void GXSkydomeObject::destroy() {
		for (GXint32 i = 0; i < components.size(); i++) {
			components[i]->destroy();
		}
	}

}