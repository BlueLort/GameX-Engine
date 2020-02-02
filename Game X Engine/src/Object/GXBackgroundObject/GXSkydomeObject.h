#pragma once
#include "Object/GXMeshObject/GXModelObject.h"
namespace gx {
	//Model Object is Object with RendererComponent and Assimp Loaded Object.
	class GX_DLL GXSkydomeObject : public GXModelObject {
	public:
		virtual void GLinit(const char* fileName);
		virtual void update(float deltaTime) override;
		virtual void destroy() override;
	private:
		GXMat4 staticModelMat4;
	};

}