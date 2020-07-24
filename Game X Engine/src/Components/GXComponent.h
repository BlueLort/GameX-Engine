#pragma once
#include "Config.h"
#include "Renderer/Shader/GXShader.h"
#include "Renderer/Buffers/GXGraphicsBufferManager.h"
#include "Renderer/GXRenderer.h"
namespace gx {
	enum GXComponentType
	{
		GX_COLLIDER=0,
		GX_RIGIDBODY,
		GX_SPRITE,
		GX_MESH,
		GX_DEBUG
		//ADD MORE LATER
	};

	enum GXComponentClass {
		GX2D = 1,
		GX_PHYSICS = 1 << 1,
		GX_AUDIO = 1 << 2,
		GX_GRAPHICS = 1 << 3,//Materials , Mesh
		GX_ANIMATION = 1 << 4
	};
	class GX_DLL GXComponent {
	public:
		uint64_t componentID;
		virtual inline const char* getName() const = 0;
		virtual inline GXuint32 getEventType() const = 0;
		virtual inline GXuint32 getEventClass() const = 0;
		virtual void update(GXFloat deltaTime) = 0;
		virtual void onGUIRender() {};
		virtual void draw(GXShader* shader, RenderType type,bool isWireFrame) {};
		virtual void draw(GXShader* shader, RenderType type, bool isWireFrame,GXuint32 nInstance) {};
		virtual void destroy() = 0;

		inline bool isSameClassAs(GXComponentClass componentClass) const {
			return this->getEventClass() == componentClass;
		}

		virtual inline const char* toString() const { return this->getName(); }
		virtual void setOwnerID(GXuint32 GXID) {
			objectID = GXID;
		}
	protected:
		static uint64_t counter;
		GXuint32 objectID;
		GXComponent(GXuint32 GXID) :componentID(counter++),objectID(GXID) {

		}
	};


}