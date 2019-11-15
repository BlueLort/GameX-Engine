#include "pch.h"
#include "GXModelObject.h"

namespace gx {




	void GXModelObject::init(const char* filePath, const char* shaderPath) {

	}

	void GXModelObject::update(float deltaTime) {
		for (auto component : components) {
			component->update(deltaTime);
		}
	}

	void GXModelObject::destroy() {
		for (int i = 0; i < meshRenderIDs.size(); i++) {
			meshRenderIDs[i].destroy();
		}
	}

}