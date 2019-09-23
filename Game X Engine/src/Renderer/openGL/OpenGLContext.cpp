#include "pch.h"
#include "OpenGLContext.h"

namespace gx {
	SDL_GLContext OpenGLContext::glContext;


	
	void OpenGLContext::init() {
		GXWindow::setAttr_GL(SDL_GL_CONTEXT_FLAGS, 0);
		GXWindow::setAttr_GL(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		GXWindow::setAttr_GL(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		GXWindow::setAttr_GL(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		GXWindow::setAttr_GL(SDL_GL_DOUBLEBUFFER, 1);
		GXWindow::setAttr_GL(SDL_GL_DEPTH_SIZE, 24);
		GXWindow::setAttr_GL(SDL_GL_STENCIL_SIZE, 8);

		glContext = SDL_GL_CreateContext(GXWindow::getSDLWindow());
		SDL_GL_MakeCurrent(GXWindow::getSDLWindow(), glContext);
		GXWindow::setSwapInterval_GL(1);

		int gladLoad = gladLoadGLLoader(SDL_GL_GetProcAddress);
		GXE_ASSERT(gladLoad, "Failed to init GLAD ... ");


		glViewport(0, 0, GXWindow::windowData->width, GXWindow::windowData->height);
	}

}