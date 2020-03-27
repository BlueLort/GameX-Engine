#pragma once
#include "Object/GXObject.h"
#include "Physics/PhysicsManager.h"
#include "Components/Physics/GXPhysicsComponent.h"
namespace gx {
	//Model Object is Object with RendererComponent and Assimp Loaded Object.
	class GX_DLL GXModelObject : public GXObject {
	public:
		inline GXModelObject(const char* objName = "NONE"):GXObject(GX_MODEL,objName){}
		virtual void GLinit(const char* fileName,const char* shaderPath=nullptr);
		virtual void update(float deltaTime) override;
		virtual void destroy() override;
		inline void fixComponentsID() {
			for (auto& comp : components) {
				comp->setOwnerID(this->GXID);
			}
		}

	};

}