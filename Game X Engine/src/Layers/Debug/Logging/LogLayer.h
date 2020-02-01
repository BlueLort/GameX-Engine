#pragma once
#include "Config.h"
#include "Layers/Layer.h"
#include "Object/GXMeshObject/GXModelObject.h"

namespace gx {
	class GX_DLL LogLayer :public Layer {

	public:
		inline LogLayer(const std::string& layerName) : Layer(layerName) {
			AutoScroll = true;
			clear();
		}
		virtual void init()override;
		virtual void destroy()override;
	
		virtual void start()override;
		virtual void end()override;
		virtual int onEvent(const gx::event::GXEventType& eventType)override;
		virtual void onUpdate(float deltaTime)override;
		virtual void onGUIRender()override;
		inline void addLog(const char* fmt, ...) IM_FMTARGS(2)
		{
			int old_size = Buf.size();
			va_list args;
			va_start(args, fmt);
			Buf.appendfv(fmt, args);
			va_end(args);
			for (int new_size = Buf.size(); old_size < new_size; old_size++)
				if (Buf[old_size] == '\n')
					LineOffsets.push_back(old_size + 1);
		}

	private:
		inline void clear()
		{
			Buf.clear();
			LineOffsets.clear();
			LineOffsets.push_back(0);
		}
		ImGuiWindowFlags windowFlags;
		int32_t width, height;
		ImGuiTextBuffer     Buf;
		ImGuiTextFilter     Filter;
		ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
		bool                AutoScroll;     // Keep scrolling if already at the bottom
	};

}
