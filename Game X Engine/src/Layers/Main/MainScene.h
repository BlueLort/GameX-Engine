#pragma once
#include "Config.h"
#include "Layers/Layer.h"
#include "Object/GXMeshObject/GXModelObject.h"
#include "Object/GXBackgroundObject/GXSkydomeObject.h"

namespace gx {
	class GX_DLL MainScene :public Layer {
	public:
		inline MainScene(const std::string& layerName) : Layer(layerName) {}
		virtual void init(int width, int height)override;
		virtual void destroy()override;
		inline void addModelObject(std::shared_ptr<GXModelObject>& obj) { sceneObjects.emplace_back(obj); };
		
		virtual void start()override;
		virtual void end()override;
		virtual int onEvent(const gx::event::GXEventType& eventType)override;
		virtual void onUpdate(float deltaTime)override;
		virtual void onGUIRender()override;

	private:
		std::vector<std::shared_ptr<GXModelObject>> sceneObjects;
		std::shared_ptr<GXSkydomeObject> skydome;
		ImGuiWindowFlags windowFlags;
		int32_t width, height;
		//just the openGL FBO for now
		std::vector<GLenum> GLFlags;
		std::unique_ptr<GLFrameBuffer> GLFBO;
	};

}
