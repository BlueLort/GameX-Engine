#include <pch.h>
#include "GXWindow.h"

namespace gx {
	SDL_Window* GXWindow::window = nullptr;
	std::shared_ptr<GXWindowData> GXWindow::windowData = nullptr;

	void GXWindow::init(const std::string& Title, uint32_t Width, uint32_t Height, uint32_t Flags)
	{
		int32_t SDLStatus = SDL_Init(SDL_INIT_VIDEO);//inits video and events
		GXE_ASSERT(SDLStatus == 0, "Failed to initialize SDL VIDEO ...\n", SDL_GetError());

		GXWindow::windowData = std::make_shared<GXWindowData>(Width, Height, Flags, Title);

		GXWindow::window = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, Flags);
		GXE_ASSERT(GXWindow::window != nullptr, "Failed to create SDL Window ...\n", SDL_GetError());
	}
}

