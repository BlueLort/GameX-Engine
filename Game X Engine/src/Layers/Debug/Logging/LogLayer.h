#pragma once
#include "Config.h"
#include "Layers/Layer.h"
#include "Object/GXMeshObject/GXModelObject.h"

namespace gx {
	static char* strdup(const char* str) { size_t len = strlen(str) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)str, len); }
	class GX_DLL LogLayer :public Layer {

	public:
		inline LogLayer(const std::string& layerName) : Layer(layerName) {
			autoScroll = true;
			clear();
		}
		virtual void init()override;
		virtual void destroy()override;
	
		virtual void start()override;
		virtual void end()override;
		virtual GXint32 onEvent(const gx::event::GXEventType& eventType)override;
		virtual void onUpdate(float deltaTime)override;
		virtual void onGUIRender()override;
		inline void addLog(const char* fmt, va_list& args) IM_FMTARGS(2)
		{
			char buf[1024];
			vsnprintf(buf, 1024, fmt, args);
			buf[1023] = 0;
			items.push_back(strdup(buf));
		}

	private:
		inline void clear()
		{
			for (GXint32 i = 0; i < items.Size; i++)
				free(items[i]);
			items.clear();
		}
		ImGuiWindowFlags windowFlags;
		GXint32 width, height;
		ImGuiTextBuffer     imguibuf;
		ImVector<GXint32>       lineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
		ImVector<char*>       items;			//items in the buffer
		bool                autoScroll;     // Keep scrolling if already at the bottom
	};

}
