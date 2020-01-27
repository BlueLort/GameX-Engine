#pragma once
#include "../GXMeshObject/GXModelObject.h"
namespace gx {
	//Model Object is Object with RendererComponent and Assimp Loaded Object.
	class GX_DLL GXSkydomeObject : public GXModelObject {
	public:
		virtual void GLinit(const char* filePath, const char* fileName);
		virtual void update(float deltaTime) override;
		virtual void destroy() override;
		void setShader(GLShader* sh) { this->glshader = sh; }
	private:
		GLShader* glshader;
		GXMat4 staticModelMat4;
	};

}