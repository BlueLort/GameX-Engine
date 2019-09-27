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
		inline void startFrame() {
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(gx::GXWindow::getSDLWindow());
			ImGui::NewFrame();
		}
		void begin();
		void setGUI();
		void render();
		void end();
		inline void demoGUI() { bool show_demo_window; ImGui::ShowDemoWindow(&show_demo_window); }
	};
	inline void ImGUI_SDLGL::begin()
	{
		ImGui::Begin("GX Editor");
	}

	inline void ImGUI_SDLGL::setGUI()
	{
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	inline void ImGUI_SDLGL::render()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	inline void ImGUI_SDLGL::end()
	{
		ImGui::End();
	}
}

