#include "pch.h"
#include "NoiseGeneratorLayer.h"

namespace gx {
	bool NoiseGeneratorLayer::isUpdated = false;
	bool NoiseGeneratorLayer::canUpdate = true;
	bool NoiseGeneratorLayer::errorLoading = false;
	uint8_t* NoiseGeneratorLayer::heightValues = nullptr;
	float* NoiseGeneratorLayer::heightsNormalized = nullptr;
	int NoiseGeneratorLayer::texWidth;
	int NoiseGeneratorLayer::texHeight;
	
	void NoiseGeneratorLayer::init()
	{
	}

	void NoiseGeneratorLayer::destroy()
	{

	}

	void NoiseGeneratorLayer::start()
	{
	}

	void NoiseGeneratorLayer::end()
	{
	}

	int NoiseGeneratorLayer::onEvent(const gx::event::GXEventType& eventType)
	{
		return 0;
	}

	void NoiseGeneratorLayer::onUpdate(float deltaTime)
	{
		if (isUpdated) {
			isUpdated = false;
			canUpdate = true;
			if(this->texID!=0)GLTexture2D::destroy(this->texID);
			GLTexture2D tex;
			tex.init(heightValues, texWidth, texHeight,GX_RGB, GX_HEIGHT);
			this->texID = tex.getID();
		}
	}

	void NoiseGeneratorLayer::onGUIRender()
	{
		ImGui::Begin(name.c_str());
		selected = ImGui::IsWindowFocused();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		static bool autoUpdate=false;
		ImGui::Text("Perlin Noise Generator Settings");
		ImGui::Checkbox("Auto-Update", &autoUpdate);
		if (autoUpdate&&canUpdate) {
			if (ImGui::SliderFloat("Scale", &scale, 2.0f, 40.0f)) {createPerlinTex();}
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
		if (ImGui::Button("Update")&& canUpdate) {
			resetHeightValues();
			createPerlinTex(); 
		}

		ImGui::Separator();
		ImGui::Text("Height Map Settings");
		ImGui::InputText("File Path", filePathBuffer, 1024);
		if (ImGui::Button("Load File")&&canUpdate) {
			resetHeightValues();
			createHMapTex(filePathBuffer);
		}
		if (errorLoading) {
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 0, 0, 255), "Error Loading File ...");
		}

		ImGui::Image(reinterpret_cast<void*>(this->texID), ImGui::GetContentRegionAvail()
			, ImVec2(1, 1), ImVec2(0, 0));
		ImGui::PopStyleVar(3);
		ImGui::End();
		GLTexture2D::stop();

	}

}