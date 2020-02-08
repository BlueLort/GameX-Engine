#pragma once
#include "Config.h"
#include "Object/GXObject.h"
namespace gx {
	class GX_DLL GXPlane : public GXObject {
	public:
		GXPlane(int Width,int Depth,const char* shaderPath=nullptr): GXObject(GX_PLANE),width(Width),depth(Depth),indices(nullptr){
			if (shaderPath == nullptr) {
				glshader = GLShaderManager::getShader(GLShaderType::DEFAULT_LIGHT);
			}
			else {
				glshader = GLShaderManager::getShader(shaderPath);
			}
		}
		void init();
		virtual void update(float deltaTime) override;
		virtual void destroy() override;
		~GXPlane() { if (indices != nullptr)delete[] indices; }
	protected:
		int width;
		int depth;
		uint32_t* indices;
		
	};
}