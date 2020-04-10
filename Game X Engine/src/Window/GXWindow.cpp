#include "pch.h"
#include "GXWindow.h"

namespace gx {
	SDL_Window* GXWindow::window = nullptr;
	std::shared_ptr<GXWindowData> GXWindow::windowData = nullptr;

	void GXWindow::init(const std::string& Title, GXuint32 Width, GXuint32 Height, GXuint32 Flags)
	{
		GXint32 SDLStatus = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);//inits video and events
		GXE_ASSERT(SDLStatus == 0, "Failed to initialize SDL2.0 Video,Timer or Events ...\n", SDL_GetError());
		
		GXWindow::window = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, Flags);
		GXE_ASSERT(GXWindow::window != nullptr, "Failed to create SDL Window ...\n", SDL_GetError());

		GXWindow::windowData = std::make_shared<GXWindowData>(Width, Height, Flags, Title, SDL_GetWindowID(window));


		
	}

	void GXWindow::destroy()
	{
	#ifdef USING_OPENGL
		SDL_GL_DeleteContext(OpenGLContext::getGLContext());
	#endif // USING_OPENGL
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	
}

