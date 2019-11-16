#pragma once
#include "pch.h"
#include "Config.h"
#include "Maths/GXTransform.h"
#include "Components/Component.h"
namespace gx {
	constexpr uint32_t MAX_COMPONENTS = 16;
	class GXComponent;
	class GX_DLL GXObject {
	public:
		GXTransform transform;
		virtual void update(float deltaTime) = 0;
		virtual void destroy() = 0;
		
		virtual inline void GLDraw() = 0;

		inline void addComponent(const std::shared_ptr<GXComponent>& component) {
			components.push_back(component);
		}
	
	protected:
		std::vector<std::shared_ptr<GXComponent>> components;
	};

}