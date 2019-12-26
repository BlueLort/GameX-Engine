#pragma once
#include "../GXObject.h"
namespace gx {
	//Model Object is Object with RendererComponent and Assimp Loaded Object.
	class GX_DLL GXModelObject : public GXObject {
	public:
		virtual void GLinit(const char* filePath, const char* fileName,const char* shaderPath=nullptr);
		virtual void update(float deltaTime) override;
		virtual void destroy() override;
		void setShader(GLShader* sh) { this->glshader = sh; }
	private:
		GLShader* glshader;

	};

}