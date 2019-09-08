#pragma once
#include <pch.h>
#include <Config.h>
#include <SDL/SDL.h>
#include "../Logging/Logger.h"
namespace gx {
	struct GXWindowData {
		uint32_t width;
		uint32_t height;
		uint32_t flags;
		std::string title;
		GXWindowData(uint32_t Width, uint32_t Height,uint32_t Flags,const std::string& Title) :width(Width), height(Height),flags(Flags), title(Title) {
		}
	};
	class GX_DLL GXWindow {

	public:
		static void init(const std::string& Title, uint32_t Width, uint32_t Height,uint32_t Flags);

		GXWindow() = delete;
	private:
		static SDL_Window* window;
		static std::shared_ptr<GXWindowData> windowData;

	};


}