#pragma once
#include "Config.h"
#include "Renderer/openGL/OpenGLContext.h"
#include "IMGUI_FILES/imgui.h"
#include "IMGUI_FILES/imgui_impl_sdl.h"
#include "IMGUI_FILES/imgui_impl_opengl3.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
extern inline void setMouseButton(GXuint32 button, bool val);
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
		template<class T>
		inline static GXint32 handleEvent(std::shared_ptr<T>& Event){ return 0; }
	private:
		void showDockspace();

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
	inline GXint32 ImGUI_SDLGL::handleEvent<gx::event::WindowCloseEvent>(std::shared_ptr<gx::event::WindowCloseEvent> & Event) {
		if (ImGuiViewport * viewport = ImGui::FindViewportByPlatformHandle((void*)SDL_GetWindowFromID(Event->windowID))) {
			viewport->PlatformRequestClose = true;
			return 1;	
		}
			return 0;
		}
	template<>
	inline GXint32 ImGUI_SDLGL::handleEvent<gx::event::WindowResizeEvent>(std::shared_ptr<gx::event::WindowResizeEvent>& Event){
		if (ImGuiViewport * viewport = ImGui::FindViewportByPlatformHandle((void*)SDL_GetWindowFromID(Event->windowID))) {
			viewport->PlatformRequestResize = true;
			return 1;
		}
		return 0;
	}
	template<>
	inline GXint32 ImGUI_SDLGL::handleEvent<gx::event::WindowMoveEvent>(std::shared_ptr<gx::event::WindowMoveEvent>& Event){
		if (ImGuiViewport * viewport = ImGui::FindViewportByPlatformHandle((void*)SDL_GetWindowFromID(Event->windowID))) {
			viewport->PlatformRequestMove= true;
			return 1;
		}
		return 1;
	}
	template<>
	inline GXint32 ImGUI_SDLGL::handleEvent<gx::event::KeyPressEvent>(std::shared_ptr<gx::event::KeyPressEvent>& Event) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[Event->getKeyVal()] = true;
		io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
		io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
		io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
		io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
		return 1;
	}
	template<>
	inline GXint32 ImGUI_SDLGL::handleEvent<gx::event::KeyReleaseEvent>(std::shared_ptr<gx::event::KeyReleaseEvent>& Event) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[Event->getKeyVal()] = false;
		io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
		io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
		io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
		io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
		return 1;
	}
	template<>
	inline GXint32 ImGUI_SDLGL::handleEvent<gx::event::KeyTypedEvent>(std::shared_ptr<gx::event::KeyTypedEvent>& Event) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharactersUTF8(Event->getTextVal());
		return 1;
	}
	template<>
	inline GXint32 ImGUI_SDLGL::handleEvent<gx::event::MouseScrollEvent>(std::shared_ptr<gx::event::MouseScrollEvent>& Event){
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += Event->getXOffset() > 0 ? 1 : -1;
		io.MouseWheel += Event->getYOffset() > 0 ? 1 : -1;
		return 1;
	}
	template<>
	inline GXint32 ImGUI_SDLGL::handleEvent<gx::event::MousePressEvent>(std::shared_ptr<gx::event::MousePressEvent>& Event) {
		setMouseButton(Event->getKeyVal() - 1, true);
		return 1;
	}

	

}

