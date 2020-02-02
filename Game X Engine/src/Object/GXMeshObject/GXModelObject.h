#pragma once
#include "Object/GXObject.h"
namespace gx {
	//Model Object is Object with RendererComponent and Assimp Loaded Object.
	class GX_DLL GXModelObject : public GXObject {
	public:
		inline GXModelObject():GXObject(GX_MODEL){}
		virtual void GLinit(const char* fileName,const char* shaderPath=nullptr);
		virtual void update(float deltaTime) override;
		virtual void destroy() override;


	};

}