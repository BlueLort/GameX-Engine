#pragma once
#include "pch.h"
#include "Config.h"
#include "Maths/GXTransform.h"
#include "Components/GXComponent.h"
#include "Renderer/Buffers/GXGraphicsBufferManager.h"
#include "Renderer/GXRenderer.h"
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
	static GXuint32 indices[] = {
		0,1,2,
		1,3,2
	};
	//QUAD FOR BEING USED AS TEXTURE RENDERER
	class GX_DLL GXQuad {
	public:
		inline GXQuad():quadVAO(0),quadVBO(0),quadEBO(0),TEXID(0) {
			TEXID = currentID;
			currentID++;
			graphicsBufferManager.reset(new GXGraphicsBufferManager());
			graphicsBufferManager->initFull(quadVertices, sizeof(quadVertices), 5 * sizeof(float));
			graphicsBufferManager->uploadIndicesToBuffer(indices,sizeof(indices), 6);
			graphicsBufferManager->setAttribPointer(0, 3, GX_FLOAT, 0);
			graphicsBufferManager->setAttribPointer(1, 2, GX_FLOAT, 3*sizeof(float));
			graphicsBufferManager->endStream();
		}
		//GXTransform transform;
		virtual void draw(const GXShader* shader);
		virtual void destroy();

		inline GXuint32 getID()const { return TEXID; }

	protected:
		GXuint32 quadVAO;
		GXuint32 quadVBO;
		GXuint32 quadEBO;
		GXuint32 TEXID;

	private:
		static GXuint32 currentID;
		std::shared_ptr<gx::GXGraphicsBufferManager> graphicsBufferManager;
	};



}