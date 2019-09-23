#pragma once

inline void initSystems(gx::GameXApp* gameInst);
int main(int argc, char** argv) {
	gx::GameXApp* Game = gx::CreateApp();
	initSystems(Game);
	Game->Start();
	delete Game;

	return 0;
}
constexpr uint32_t WINDOW_WIDTH = 1024;
constexpr uint32_t WINDOW_HEIGHT = 768;
constexpr uint32_t SDL_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
//init different components of the engine
void initSystems(gx::GameXApp* gameInst) {
	gx::Logger::init();	GXE_INFO("Logger initialized successfully !");
	gx::GXWindow::init("Game X Engine", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_FLAGS); GXE_INFO("Window initialized successfully using SDL2.0 !");
	gx::GXWindow::addEventsCallBack(gameInst->getEventCallBack(), nullptr); GXE_INFO("Event CallBack Attached to SDL2.0 Event Queue !");
	gameInst->timer->init(); GXE_INFO("Application Timer Started using SDL_GetTicks !");
#ifdef USING_OPENGL
	gx::OpenGLContext::init(); GXE_INFO("OpenGL Context initialized using GLAD as Loader!");
	GXE_PRINT(gx::OpenGLContext::getGLVersion());
	GXE_PRINT(gx::OpenGLContext::getGPUName());
	GXE_PRINT(gx::OpenGLContext::getVendorName());
	gameInst->ImGUIGL_Layer->init(); GXE_INFO("dear ImGUI v1.73 WIP initialized for OpenGL3.0 !");
#else

#endif 

	
}


