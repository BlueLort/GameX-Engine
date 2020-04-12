#pragma once
#include "pch.h"
#include "Config.h"
#include "Maths/GXTransform.h"
#include "Components/GXComponent.h"
#include "Renderer/Buffers/GXGraphicsBufferManager.h"
#include "Renderer/Shader/GXShaderManager.h"
#include "IOManagement/IOManager.h"
#include "Renderer/GXRenderer.h"
#include "Camera/EditorCamera/EditorCamera.h"
#include "Light/SceneLightManager.h"
#include "IOManagement/IORequestHandler.h"

namespace gx {
	enum GXObjectType { GX_MODEL, GX_SKYDOME,GX_PLANE };
	class GX_DLL GXObject {
	public:
		inline GXObject(GXObjectType ObjectType,const char* objName) :objType(ObjectType), isReady(false),isWireFrame(false){
			this->GXID = currentID++;
			this->objName = std::string(objName);
		}
		GXTransform transform;

		virtual void init(const char* fileName, const char* shaderPath = nullptr) = 0;
		virtual void update(float deltaTime) = 0;
		virtual void destroy() = 0;

		inline void addComponent(const std::shared_ptr<GXComponent>& component) {
			components.emplace_back(component);
		}

		inline GXObjectType getType()const { return objType; }
		inline GXuint32 getID()const { return GXID; }
		inline void setShader(GXShader* sh) { this->shader = sh; }
		inline GXShader* getShader() const { return shader; }
		inline const char* getName() const { return objName.c_str(); }
		
		bool isReady;
		bool isWireFrame;
	protected:
		std::vector<std::shared_ptr<GXComponent>> components;
		GXObjectType objType;
		GXShader* shader;
		GXuint32 GXID;
		std::string objName;
	private:
		static GXuint32 currentID;
	};
	


}