#pragma once
#include "Config.h"
#include "Window/Layer.h"
#include "Renderer/openGL/OpenGLContext.h"
#include "IMGUI_FILES/imgui.h"
#include "IMGUI_FILES/imgui_impl_sdl.h"
#include "IMGUI_FILES/imgui_impl_opengl3.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
namespace gx {
	class GX_DLL ImGUI_SDLGL : public Layer
	{
	public:
		inline ImGUI_SDLGL(const std::string& layerName) : Layer(layerName) {}
		virtual void init() override;
		virtual void destroy() override ;
		virtual void onEvent(const gx::event::GXEvent& event) override;
		virtual void onUpdate(float deltaTime) override;

	};
}

