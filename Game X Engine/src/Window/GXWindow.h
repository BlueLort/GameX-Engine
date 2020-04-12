#pragma once
#include "pch.h"
#include "Config.h"
namespace gx {
	struct GX_DLL GXWindowData {
		GXuint32 width;
		GXuint32 height;
		GXuint32 flags;
		std::string title;
		GXuint32 id;
		GXWindowData(GXuint32 Width, GXuint32 Height, GXuint32 Flags, const std::string& Title,GXuint32 ID) :width(Width), height(Height), flags(Flags), title(Title),id(ID) {
		}
	};
	class GX_DLL GXWindow {

	public:
		static void init(const std::string& Title, GXuint32 Width, GXuint32 Height, GXuint32 Flags);
		inline static void addEventsCallBack(SDL_EventFilter filter, void* userdata) { SDL_AddEventWatch(filter, userdata); }
		static void destroy();
		inline static SDL_Window* getSDLWindow() { return window; }

#ifdef USING_OPENGL
		//SDL OPEN_GL SPECIFIC
		static void setAttr(SDL_GLattr flag, GXint32 val){ SDL_GL_SetAttribute(flag, val); }
		static void setSwapInterval(GXint32 val){ SDL_GL_SetSwapInterval(val); }
		static void swapWindow(){ SDL_GL_SwapWindow(window); }
#endif
		static std::shared_ptr<GXWindowData> windowData;
		GXWindow() = delete;
	private:
		static SDL_Window* window;
	

	};


}