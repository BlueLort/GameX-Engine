#include "pch.h"
#include "GXSkydomeObject.h"

namespace gx {

	void GXSkydomeObject::GLinit(const char* fileName) {

		glshader = GLShaderManager::getShader(GLShaderType::DEFAULT_SKYDOME);
		transform.scale = GXVec3(3000.0f, 3000.0f, 3000.0f);//enlarge the sphere
		staticModelMat4 = transform.getModel();
		//GET THE MODEL DATA
		io::IORequestHandler::getModel(fileName, &components, &isReady);
	}

	void GXSkydomeObject::update(float deltaTime) {
		//For rendering as rendering happens in GXMeshComponent
#ifdef USING_OPENGL
		//First set the proper depth func to allow us draw skydome at the end of the frame
		GLRenderer::getInstance().setDepthFunc(DepthFunc::GX_LEQUAL);
		GLRenderer::getInstance().setWindingOrder(WindingOrder::GX_CW);//must be CW because we are inside it
		this->glshader->use();
		this->glshader->setMat4("model", staticModelMat4);
		this->glshader->setMat4("view", EditorCamera::getInstance().getViewMatrix());
		this->glshader->setMat4("projection", EditorCamera::getInstance().getProjectionMatrix());
#endif
		for (auto component : components) {
			component->update(deltaTime);
			component->draw(glshader);
		}
#ifdef USING_OPENGL
		//reset values to default
		GLRenderer::getInstance().setDepthFunc(DepthFunc::GX_LESS);
		GLRenderer::getInstance().setWindingOrder(WindingOrder::GX_CCW);
#endif
	}

	void GXSkydomeObject::destroy() {
		for (int i = 0; i < components.size(); i++) {
			components[i]->destroy();
		}
	}

}