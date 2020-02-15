#pragma once
#include "pch.h"
#include "Config.h"
#include "Maths/GXTransform.h"
#include "Components/GXComponent.h"
#include "Renderer/openGL/Buffers/GLBufferManager.h"
#include "IOManagement/IOManager.h"
#include "Renderer/openGL/GLRenderer.h"
#include "Camera/EditorCamera/EditorCamera.h"
#include "Light/SceneLightManager.h"
#include "IOManagement/IORequestHandler.h"
namespace gx {
	enum GXObjectType { GX_MODEL, GX_SKYDOME,GX_PLANE };
	class GX_DLL GXObject {
	public:
		inline GXObject(GXObjectType ObjectType) :objType(ObjectType), isReady(false),isWireFrame(false){

		}
		GXTransform transform;
		virtual void update(float deltaTime) = 0;
		virtual void destroy() = 0;

		inline void addComponent(const std::shared_ptr<GXComponent>& component) {
			components.emplace_back(component);
		}
		inline GXObjectType getType()const { return objType; }
		void setShader(GLShader* sh) { this->glshader = sh; }
		GLShader* getShader() const { return glshader; }

		bool isReady;
		bool isWireFrame;
	protected:
		std::vector<std::shared_ptr<GXComponent>> components;
		GXObjectType objType;
		GLShader* glshader;
	};
	
	

}