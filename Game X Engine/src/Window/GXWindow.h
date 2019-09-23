#pragma once
#include "pch.h"
#include "Config.h"
#include "Renderer/openGL/OpenGLContext.h"
namespace gx {
	struct GXWindowData {
		uint32_t width;
		uint32_t height;
		uint32_t flags;
		std::string title;
		GXWindowData(uint32_t Width, uint32_t Height, uint32_t Flags, const std::string& Title) :width(Width), height(Height), flags(Flags), title(Title) {
		}
	};
	class OpenGLContext;
	class GX_DLL GXWindow {

	public:
		static void init(const std::string& Title, uint32_t Width, uint32_t Height, uint32_t Flags);
		static void addEventsCallBack(SDL_EventFilter filter, void* userdata) { SDL_AddEventWatch(filter, userdata); }
		static void destroy();
		static SDL_Window* getSDLWindow() { return window; }
#ifdef USING_OPENGL
		static void setAttr_GL(SDL_GLattr flag, int32_t val){ SDL_GL_SetAttribute(flag, val); }
		static void setSwapInterval_GL(int32_t val){ SDL_GL_SetSwapInterval(val); }
		static void swapWindow_GL(){ SDL_GL_SwapWindow(window); }
#endif
		static std::shared_ptr<GXWindowData> windowData;
		GXWindow() = delete;
	private:
		static SDL_Window* window;
	

	};


}