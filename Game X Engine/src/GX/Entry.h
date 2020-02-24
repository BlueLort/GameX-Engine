#pragma once

inline void initSystems(gx::GameXApp* gameInst);
int main(int argc, char** argv) {
	gx::GameXApp* Game = gx::CreateApp();
	initSystems(Game);
	Game->Start();
	//TODO Destroy Core Systems + Resources Systems [like IOManager] in reverse order of init... 
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
	gx::InputManager::getInstance().init(); GXE_INFO("InputManager initialized successfully!");
	gx::SceneLightManager::getInstance().init();  GXE_PRINT("Lighting information is Initialized for debugging");
#ifdef USING_GLM_MATHS
	GXE_INFO("Using GLM for Maths library!");
#endif
	gx::GXPhysicsManager::getInstance().init(); GXE_INFO("Physics Engine Initialized [Bullet Physics] !");
	gx::GXTimer::getAppTimer().init(); GXE_INFO("Application Timer Started using SDL_GetTicks !");
	gx::EditorCamera::getInstance().init(); GXE_INFO("Editor Camera initialized !");
	//init Graphics Context and appropriate Rendering System
#ifdef USING_OPENGL
	GXE_INFO("Using OpenGL as rendering API !");
	gx::GLRenderer::getInstance().init(); GXE_INFO("GLRenderer is Initialized Successfully !");
	gameInst->UI_GL->init(); GXE_INFO("dear ImGUI v1.73 WIP initialized for OpenGL !");
	GXE_PRINT("Initiating GLShaders");
	gx::GLShaderManager::init(); GXE_INFO("OpenGL Shader Manager initialized !");

#endif 
	//preload models
	gx::io::IORequestHandler::importModel("res/models/sphere/", "spheres.obj");
	gx::io::IORequestHandler::waitTasks();
	gx::LayerManager::getInstance().init();//instantiate a layer manager.[first ref]
	GXE_INFO("Layers Manager Initialized !");

}


