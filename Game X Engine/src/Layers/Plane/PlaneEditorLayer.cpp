#include "pch.h"
#include "PlaneEditorLayer.h"

namespace gx {
	bool PlaneEditorLayer::isUpdated = false;
	bool PlaneEditorLayer::canUpdate = true;
	uint8_t* PlaneEditorLayer::texColors = nullptr;
	std::shared_ptr<GXPlane> PlaneEditorLayer::plane;
	constexpr GXint32 MAX_WIDTH = 8192;
	constexpr GXint32 MAX_DEPTH = 8192;
	constexpr GXint32 MAX_HEIGHT = 128;
	constexpr GXint32 MAX_INSTANCESX = 32;
	constexpr GXint32 MAX_INSTANCESZ = 32;
	constexpr GXint32 MAX_DENSITYX = 32;
	constexpr GXint32 MAX_DENSITYZ = 32;
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

	GXint32  PlaneEditorLayer::onEvent(const gx::event::GXEventType& eventType)
	{
		return 0;
	}

	void  PlaneEditorLayer::onUpdate(GXFloat deltaTime)
	{
		if (isUpdated) {
			isUpdated = false;
			
			if (this->texID != 0)GXTexture2D::destroy(this->texID);
			GXTexture2D tex;
			tex.init(texColors, NoiseGeneratorLayer::texWidth, NoiseGeneratorLayer::texHeight, GX_RGB, GX_RGB, GX_DIFFUSE, GXTexture2DFilteringMethod::GX_NEAREST, GX_UNSIGNED_BYTE);
			this->texID = tex.getID();
			plane->uploadToBuffer(texID, NoiseGeneratorLayer::texID);
			plane->isReady = true;
			//plane->isWireFrame = true;
			MainScene::mainPlane = plane;
			canUpdate = true;
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
		ImGui::DragInt("Patch Width", &width, 1, 1, MAX_WIDTH, "%d%");
		ImGui::DragInt("Patch Depth", &depth, 1, 1, MAX_DEPTH, "%d%");
		ImGui::DragInt("Max Height", &height, 1, 0, MAX_HEIGHT, "%d%");
		ImGui::DragInt("# XInstances", &colInstances, 1, 1, MAX_INSTANCESX, "%d%");
		ImGui::DragInt("# ZInstances", &rowInstances, 1, 1, MAX_INSTANCESZ, "%d%");
		ImGui::DragInt("Patch X Density", &xDensity, 1, 2, MAX_DENSITYX, "%d%");
		ImGui::DragInt("Patch Z Density", &zDensity, 1, 2, MAX_DENSITYZ, "%d%");
		ImGui::Checkbox("Use Hue Wheel", &isHueWheel);
		ImGuiColorEditFlags flags = isHueWheel ? ImGuiColorEditFlags_PickerHueWheel : ImGuiColorEditFlags_PickerHueBar;
		ImGui::ColorEdit3("Base Color", levels[0].colors, flags);
		CLAMP(width, 1, MAX_WIDTH);
		CLAMP(depth, 1, MAX_DEPTH);
		CLAMP(height, 0, MAX_HEIGHT);
		CLAMP(colInstances, 1, MAX_INSTANCESX);
		CLAMP(rowInstances, 1, MAX_INSTANCESZ);
		CLAMP(xDensity, 2, MAX_DENSITYX);
		CLAMP(zDensity, 2, MAX_DENSITYZ);
		if (ImGui::Button("Update/Add Plane") && canUpdate) {
			canUpdate = false;
			asyncTask = std::async(std::launch::async, createColoredTexture, width, depth, height, rowInstances,
				colInstances, xDensity, zDensity, levels);

		}
		ImGui::Separator();
		for (GXint32 i = 1; i < levels.size(); i++) {
			//TODO improve this IDing of ui elements as string operations are costly.
			ImGui::DragFloat(std::string("Min. Height##" + std::to_string(i)).c_str(), &levels[i].heightVal, 0.001f, 0.001f, 1.0f, "%.3f%");
			CLAMP(levels[i].heightVal, 0.001f, 1.0f);
			ImGui::ColorEdit3(std::string("Level Color##" + std::to_string(i)).c_str(), levels[i].colors, flags);
			if (ImGui::Button(std::string("Remove Level##" + std::to_string(i)).c_str())) {
				levels.erase(levels.begin() + i);
			}
			ImGui::Separator();
		}
		if (ImGui::Button("Add Level")) {
			levels.push_back(LevelData());
		}


		ImGui::Image(reinterpret_cast<void*>(this->texID), ImGui::GetContentRegionAvail()
			, ImVec2(1, 1), ImVec2(0, 0));

		ImGui::PopStyleVar(3);
		ImGui::End();
		GXTexture2D::stop();

	}

}