#pragma once
#include "pch.h"
#include "Config.h"
#include "Maths/GXTransform.h"
#include "Components/GXComponent.h"
#include "Renderer/openGL/Buffers/GLBufferManager.h"
#include "Renderer/openGL/GLRenderer.h"
#include "Camera/EditorCamera/EditorCamera.h"
#include "Light/SceneLightManager.h"
namespace gx {
	
	static float quadVertices[] = {
		// positions			// texCoords
		-1.0f,  1.0f, 0.0f,		 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,		 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,		 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,		 1.0f, 0.0f,
	};
	static uint32_t indices[] = {
		0,1,2,
		1,3,2
	};
	//QUAD FOR BEING USED AS TEXTURE RENDERER
	class GX_DLL GXQuad {
	public:
		inline GXQuad() {
			TEXID = currentID;
			currentID++;
			GLBM.reset(new GLBufferManager());
			GLBM->initFull(quadVertices, sizeof(quadVertices), 5 * sizeof(float));
			GLBM->uploadIndicesToBuffer(indices,sizeof(indices), 6);
			GLBM->setAttribPointer(0, 3, GL_FLOAT, 0);
			GLBM->setAttribPointer(1, 2, GL_FLOAT, 3*sizeof(float));
			GLBM->endStream();
		}
		//GXTransform transform;
		virtual void update(float deltaTime,const GLShader* glshader);
		virtual void destroy();

		inline uint32_t getID()const { return TEXID; }

	protected:
		uint32_t quadVAO;
		uint32_t quadVBO;
		uint32_t quadEBO;
		uint32_t TEXID;

	private:
		static uint32_t currentID;
		std::shared_ptr<gx::GLBufferManager> GLBM;
	};



}