#include "pch.h"
#include "LogLayer.h"

namespace gx {
	/*COPIED FROM IMGUI LIB*/

	void LogLayer::init()
	{
		windowFlags = 0;
	}

	void LogLayer::destroy()
	{
		

	}

	void LogLayer::start()
	{
	}

	void LogLayer::end()
	{
	
	}

	int LogLayer::onEvent(const gx::event::GXEventType& eventType)
	{
		return 0;
	}

	void LogLayer::onUpdate(float deltaTime)
	{


	}

	void LogLayer::onGUIRender()
	{
		bool copyClipboard=false;
		if (!isOpen)return;
		ImGui::Begin(name.c_str());
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &autoScroll);
			if (ImGui::Button("Clear")) {
				this->clear();
			}
			ImGui::EndPopup();
		}
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
	
		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = imguibuf.begin();
		const char* buf_end = imguibuf.end();
		for (int i = 0; i < items.Size; i++)
		{
			const char* item = items[i];
			ImGui::TextUnformatted(item);
		}
		ImGui::PopStyleVar();

		if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
		ImGui::End();

	}

}