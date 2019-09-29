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
		inline ImGUI_SDLGL(const std::string& layerName) : Layer(layerName){}
		virtual void init() override;
		virtual void destroy() override ;
		virtual void onEvent(const gx::event::GXEvent& event) override;
		virtual void onGUIRender() override;
		void startFrame();
		void render();
		void endFrame();//essential for viewport effect
	};
	inline void ImGUI_SDLGL::startFrame() {
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(gx::GXWindow::getSDLWindow());
		ImGui::NewFrame();
	}
	inline void ImGUI_SDLGL::render()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	inline void ImGUI_SDLGL::endFrame() {
		// Update and Render additional Platform Windows
	  // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	  //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}

	}
}

