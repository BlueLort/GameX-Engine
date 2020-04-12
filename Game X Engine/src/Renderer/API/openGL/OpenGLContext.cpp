#include "pch.h"
#include "OpenGLContext.h"

namespace gx {
	SDL_GLContext OpenGLContext::glContext;


	
	void OpenGLContext::init() {
		GXWindow::setAttr(SDL_GL_CONTEXT_FLAGS, 0);
		GXWindow::setAttr(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		//SDL Context for OpenGL 3.3 
		GXWindow::setAttr(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		GXWindow::setAttr(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		GXWindow::setAttr(SDL_GL_DOUBLEBUFFER, 1);
		GXWindow::setAttr(SDL_GL_DEPTH_SIZE, 24);
		GXWindow::setAttr(SDL_GL_STENCIL_SIZE, 8);

		glContext = SDL_GL_CreateContext(GXWindow::getSDLWindow());
		SDL_GL_MakeCurrent(GXWindow::getSDLWindow(), glContext);
		GXWindow::setSwapInterval(1);

		GXint32 gladLoad = gladLoadGLLoader(SDL_GL_GetProcAddress);
		GXE_ASSERT(gladLoad, "Failed to init GLAD ... ");
	}


}