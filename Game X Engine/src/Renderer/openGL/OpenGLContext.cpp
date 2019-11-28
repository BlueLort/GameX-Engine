#include "pch.h"
#include "OpenGLContext.h"

namespace gx {
	SDL_GLContext OpenGLContext::glContext;


	
	void OpenGLContext::init() {
		GXWindow::setAttr_GL(SDL_GL_CONTEXT_FLAGS, 0);
		GXWindow::setAttr_GL(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		//SDL Context for OpenGL 3.3 
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


		//TODO MAKE A SYSTEM FOR THE FOLLOWING ENABLES
		glEnable(GL_CULL_FACE);

		glViewport(0, 0, GXWindow::windowData->width, GXWindow::windowData->height);

		glClearColor(0.258f, 0.596f, 0.96f, 1.0f);
	}

	
	void OpenGLContext::begin()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

	void OpenGLContext::end()
	{
		GXWindow::swapWindow_GL();
	}


}