#pragma once
#include "Config.h"
#include "Window/Layer.h"
namespace gx {
	class GX_DLL ImGUI : public Layer
	{
		inline ImGUI(const std::string& layerName) : Layer(layerName) {}
		virtual void Init() override;
		virtual void Destroy() override ;
		virtual void onEvent(const gx::event::Event& event) override;
		virtual void onUpdate(float deltaTime) override;

	};
}

