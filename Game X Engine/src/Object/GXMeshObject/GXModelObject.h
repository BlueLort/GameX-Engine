#pragma once
#include "../GXObject.h"
#include "Renderer/openGL/Buffers/GLBufferManager.h"
namespace gx {
	//Model Object is Object with RendererComponent and Assimp Loaded Object.
	class GX_DLL GXModelObject : public GXObject {
	public:
		virtual void init(const char* filePath,const char* shaderPath=nullptr);
		virtual void update(float deltaTime) override;
		virtual void destroy() override;
	private:

#ifdef USING_OPENGL
		std::vector<GLBufferManager> meshRenderIDs;
		GLShader shader;
#endif

	};

}