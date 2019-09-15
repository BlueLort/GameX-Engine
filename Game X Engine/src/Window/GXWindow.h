#pragma once
#include "pch.h"
#include "Config.h"
namespace gx {
	struct GXWindowData {
		uint32_t width;
		uint32_t height;
		uint32_t flags;
		std::string title;
		GXWindowData(uint32_t Width, uint32_t Height, uint32_t Flags, const std::string& Title) :width(Width), height(Height), flags(Flags), title(Title) {
		}
	};
	class GX_DLL GXWindow {

	public:
		static void init(const std::string& Title, uint32_t Width, uint32_t Height, uint32_t Flags);
		static void addEventsCallBack(SDL_EventFilter filter, void* userdata) { SDL_AddEventWatch(filter, userdata); }
		static void destroy();
#ifdef USING_OPENGL
		void setAttr_GL(SDL_GLattr flag, int32_t val) const { SDL_GL_SetAttribute(flag, val); }
		void setSwapInterval_GL(int32_t val)const { SDL_GL_SetSwapInterval(val); }
		void swapWindow_GL() const { SDL_GL_SwapWindow(window); }
#endif

		GXWindow() = delete;
	private:
		static SDL_Window* window;
		static std::shared_ptr<GXWindowData> windowData;

	};


}