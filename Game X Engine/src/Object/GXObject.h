#pragma once
#include "pch.h"
#include "Config.h"
#include "Maths/GXTransform.h"
#include "Components/GXComponent.h"
#include "Renderer/openGL/Buffers/GLBufferManager.h"
#include "IOManagement/IOManager.h"
namespace gx {
	constexpr uint32_t MAX_COMPONENTS = 16;
	class GXComponent;
	class GX_DLL GXObject {
	public:
		GXTransform transform;
		virtual void update(float deltaTime) = 0;
		virtual void destroy() = 0;
		
		virtual void GLDraw(GLShader* sh) = 0;
		virtual void GLDraw() = 0;
		inline void addComponent(const std::shared_ptr<GXComponent>& component) {
			components.push_back(component);
		}
	
	protected:
		std::vector<std::shared_ptr<GXComponent>> components;
	};

}