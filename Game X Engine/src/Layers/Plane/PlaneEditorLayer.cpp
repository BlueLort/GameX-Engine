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
	constexpr GXint32 MAX_N = 8;
	constexpr GXint32 MAX_M = 8;
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
			canUpdate = true;
			if (this->texID != 0)GXTexture2D::destroy(this->texID);
			GXTexture2D tex;
			tex.init(texColors, width, depth, GX_RGB, GX_DIFFUSE, GXTexture2DFilteringMethod::GX_NEAREST);
			this->texID = tex.getID();
			plane->uploadToBuffer(texID);
			plane->isReady = true;
			//plane->isWireFrame = true;
			MainScene::mainPlane = plane;
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
		ImGui::DragInt("Quads Multiplier X", &clusteringFactorX, 0.25, 1, MAX_N, "%d%");
		ImGui::DragInt("Quads Multiplier Z", &clusteringFactorZ, 0.25, 1, MAX_M, "%d%");
		ImGui::Checkbox("Use Hue Wheel", &isHueWheel);
		ImGuiColorEditFlags flags = isHueWheel ? ImGuiColorEditFlags_PickerHueWheel : ImGuiColorEditFlags_PickerHueBar;
		ImGui::ColorEdit3("Base Color", levels[0].colors, flags);
		CLAMP(width, 2, MAX_WIDTH);
		CLAMP(depth, 2, MAX_DEPTH);
		CLAMP(height, 0, MAX_HEIGHT);
		CLAMP(clusteringFactorX, 1, MAX_N);
		CLAMP(clusteringFactorZ, 1, MAX_M);
		if (ImGui::Button("Update/Add Plane") && canUpdate) {
			canUpdate = false;
			asyncTask = std::async(std::launch::async, createColoredTexture, width, depth, height, clusteringFactorX, clusteringFactorZ, levels);

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