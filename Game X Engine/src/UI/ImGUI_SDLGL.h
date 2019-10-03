#pragma once
#include "Config.h"
#include "Window/Layer.h"
#include "Renderer/openGL/OpenGLContext.h"
#include "IMGUI_FILES/imgui.h"
#include "IMGUI_FILES/imgui_impl_sdl.h"
#include "IMGUI_FILES/imgui_impl_opengl3.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
namespace gx {
	class GX_DLL ImGUI_SDLGL
	{
	public:
		inline ImGUI_SDLGL(const std::string& layerName) {}
		virtual void init();
		virtual void destroy();
		virtual void onGUIRender();
		void startFrame();
		void render();
		void endFrame();//essential for viewport effect


		//EVENTS
		int handleEvent(std::shared_ptr<gx::event::WindowCloseEvent>& event) { return 0; }
		template<class T>
		inline static int handleEvent(std::shared_ptr<T>& Event){ return 0; }
		template<>
		static int handleEvent<gx::event::WindowCloseEvent>(std::shared_ptr<gx::event::WindowCloseEvent> & Event);
		template<>
		static int handleEvent<gx::event::WindowResizeEvent>(std::shared_ptr<gx::event::WindowResizeEvent>& Event);
		template<>
		static int handleEvent<gx::event::WindowMoveEvent>(std::shared_ptr<gx::event::WindowMoveEvent>& Event);
		template<>
		static int handleEvent<gx::event::MouseScrollEvent>(std::shared_ptr<gx::event::MouseScrollEvent>& Event);

	private:


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



	//EVENTS 
	template<>
	inline int ImGUI_SDLGL::handleEvent<gx::event::WindowCloseEvent>(std::shared_ptr<gx::event::WindowCloseEvent> & Event) {
		if (ImGuiViewport * viewport = ImGui::FindViewportByPlatformHandle((void*)SDL_GetWindowFromID(Event->getWindowID()))) {
			viewport->PlatformRequestClose = true;
			return 1;	
		}
			return 0;
		}
	template<>
	inline int ImGUI_SDLGL::handleEvent<gx::event::WindowResizeEvent>(std::shared_ptr<gx::event::WindowResizeEvent>& Event){
		if (ImGuiViewport * viewport = ImGui::FindViewportByPlatformHandle((void*)SDL_GetWindowFromID(Event->getWindowID()))) {
			viewport->PlatformRequestResize = true;
			return 1;
		}
		return 0;
	}
	template<>
	inline int ImGUI_SDLGL::handleEvent<gx::event::WindowMoveEvent>(std::shared_ptr<gx::event::WindowMoveEvent>& Event){
		if (ImGuiViewport * viewport = ImGui::FindViewportByPlatformHandle((void*)SDL_GetWindowFromID(Event->getWindowID()))) {
			viewport->PlatformRequestMove= true;
			return 1;
		}
		return 0;
	}
	template<>
	inline int ImGUI_SDLGL::handleEvent<gx::event::MouseScrollEvent>(std::shared_ptr<gx::event::MouseScrollEvent>& Event){
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += Event->getXOffset() > 0 ? 1 : -1;
		io.MouseWheel += Event->getYOffset() > 0 ? 1 : -1;
		return 0;
	}
}

