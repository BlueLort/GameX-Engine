#pragma once
#include "Config.h"
#include "Renderer/openGL/Shader/GLShader.h"
#include "Renderer/openGL/Shader/GLShader.h"
#include "Renderer/openGL/Buffers/GLBufferManager.h"
#include "Renderer/openGL/GLRenderer.h"
namespace gx {
	enum GXComponentType
	{
		GX_COLLIDER=0,
		GX_RIGIDBODY,
		GX_SPRITE,
		GX_MESH
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
		virtual inline uint32_t getEventType() const = 0;
		virtual inline uint32_t getEventClass() const = 0;
		virtual void update(float deltaTime) = 0;
		virtual void draw(GLShader* glshader,bool isWireFrame) {};
		virtual void destroy() = 0;

		inline bool isSameClassAs(GXComponentClass componentClass) const {
			return this->getEventClass() == componentClass;
		}

		virtual inline const char* toString() const { return this->getName(); }

	protected:
		static uint64_t counter;
		GXComponent() :componentID(counter++) {

		}
	};


}