#include "pch.h"
#include "PlaneEditorLayer.h"

namespace gx {
	bool PlaneEditorLayer::isUpdated = false;
	bool PlaneEditorLayer::canUpdate = true;
	constexpr int MAX_WIDTH = 8192;
	constexpr int MAX_DEPTH = 8192;
	constexpr int MAX_HEIGHT = 128;
	void  PlaneEditorLayer::init()
	{
	}

	void  PlaneEditorLayer::destroy()
	{

	}

	void  PlaneEditorLayer::start()
	{
	}

	void  PlaneEditorLayer::end()
	{
	}

	int  PlaneEditorLayer::onEvent(const gx::event::GXEventType& eventType)
	{
		return 0;
	}

	void  PlaneEditorLayer::onUpdate(float deltaTime)
	{
		if (isUpdated) {
			isUpdated = false;
			canUpdate = true;
			if (this->texID != 0)GLTexture2D::destroy(this->texID);
			GLTexture2D tex;
			tex.init(texColors, width, depth, GX_RGB, GX_HEIGHT);
			this->texID = tex.getID();
		}
	}

	void  PlaneEditorLayer::onGUIRender()
	{
		ImGui::Begin(name.c_str());
		selected = ImGui::IsWindowFocused();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		static bool isHueWheel = false;
		ImGui::DragInt("Width", &width, 1, 2, MAX_WIDTH, "%d%");
		ImGui::DragInt("Depth", &depth, 1, 2, MAX_DEPTH, "%d%");
		ImGui::DragInt("MaxHeight", &height, 1, 0, MAX_HEIGHT, "%d%");
		ImGui::DragFloat3("Scale", scale,0.01f);
		ImGui::Checkbox("Use Hue Wheel", &isHueWheel);
		ImGuiColorEditFlags flags = isHueWheel ? ImGuiColorEditFlags_PickerHueWheel : ImGuiColorEditFlags_PickerHueBar;
		ImGui::ColorEdit3("Base Color", levels[0].colors,flags);
		CLAMP(width, 2, MAX_WIDTH);
		CLAMP(depth, 2, MAX_DEPTH);
		CLAMP(height, 0, MAX_HEIGHT);
		if (ImGui::Button("Add Level")) {
			levels.push_back(LevelData());
		}
		ImGui::Separator();
		for (int i = 1; i < levels.size(); i++) {
			//TODO improve this IDing of ui elements as string operations are costly.
			ImGui::DragFloat(std::string("Min. Height##" + std::to_string(i)).c_str(), &levels[i].heightVal, 0.001f, 0.001f, 1.0f, "%.3f%");
			CLAMP(levels[i].heightVal, 0.001f, 1.0f);
			ImGui::ColorEdit3(std::string("Level Color##" + std::to_string(i)).c_str(), levels[i].colors, flags);
			ImGui::Separator();
		}
		
		





		/*
		if (autoUpdate && canUpdate) {
			if (ImGui::SliderFloat("Scale", &scale, 2.0f, 40.0f)) { createPerlinTex(); }
			if (ImGui::SliderInt("# Octaves", &nOctaves, 1, 9)) { createPerlinTex(); }
			if (ImGui::SliderFloat("Persistence", &persistence, 0.01f, 1.0f)) { createPerlinTex(); }
			if (ImGui::SliderFloat("Lacunarity", &lacunarity, 1.1f, 10.0f)) { createPerlinTex(); }
			if (ImGui::SliderFloat("Z-Plane[Seed]", &z, 0.0f, 1000.0f)) { createPerlinTex(); }

		}
		else {
			ImGui::SliderFloat("Scale", &scale, 2.0f, 40.0f);
			ImGui::SliderInt("# Octaves", &nOctaves, 1, 9);
			ImGui::SliderFloat("Persistence", &persistence, 0.01f, 1.0f);
			ImGui::SliderFloat("Lacunarity", &lacunarity, 1.1f, 10.0f);
			ImGui::SliderFloat("Z-Plane[Seed]", &z, 0.0f, 1000.0f);
		}
		if (ImGui::Button("Update") && canUpdate) {
			resetHeightValues();
			createPerlinTex();
		}

		ImGui::Separator();
		ImGui::Text("Height Map Settings");
		ImGui::InputText("File Path", filePathBuffer, 1024);
		if (ImGui::Button("Load File") && canUpdate) {
			resetHeightValues();
			createHMapTex(filePathBuffer);
		}
		if (errorLoading) {
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 0, 0, 255), "Error Loading File ...");
		}
		*/
		ImGui::Image(reinterpret_cast<void*>(this->texID), ImGui::GetContentRegionAvail()
			, ImVec2(1, 1), ImVec2(0, 0));

		ImGui::PopStyleVar(3);
		ImGui::End();
		GLTexture2D::stop();

	}

}