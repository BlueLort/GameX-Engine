#pragma once
#include "Config.h"
#include "Layers/Layer.h"
#include "Object/GXMeshObject/GXModelObject.h"
#include "Object/GXBackgroundObject/GXSkydomeObject.h"

namespace gx {
	class GX_DLL NoiseGeneratorLayer :public Layer {
	public:
		inline NoiseGeneratorLayer(const std::string& layerName) : Layer(layerName) {}
		virtual void init(int width, int height)override;
		virtual void destroy()override;
		virtual void start()override;
		virtual void end()override;
		virtual int onEvent(const gx::event::GXEventType& eventType)override;
		virtual void onUpdate(float deltaTime)override;
		virtual void onGUIRender()override;

	private:

		std::shared_ptr<GXSkydomeObject> skydome;
		ImGuiWindowFlags windowFlags;
		int32_t width, height;
	};

}
