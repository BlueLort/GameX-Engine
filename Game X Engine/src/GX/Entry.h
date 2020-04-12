#pragma once

inline void initSystems(gx::GameXApp* gameInst);
GXint32 main(GXint32 argc, char** argv) {
	gx::GameXApp* Game = gx::CreateApp();
	initSystems(Game);
	Game->Start();
	//TODO Destroy Core Systems + Resources Systems [like IOManager] in reverse order of init... 
	delete Game;

	return 0;
}
constexpr GXuint32 WINDOW_WIDTH = 1024;
constexpr GXuint32 WINDOW_HEIGHT = 768;
constexpr GXuint32 SDL_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
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
	GXE_INFO("Using OpenGL as rendering API !");
	gx::GXRenderer::getInstance().init(); GXE_INFO("GXRenderer is Initialized Successfully !");
	gameInst->gxUserInterface->init(); GXE_INFO("dear ImGUI v1.73 WIP initialized !");
	GXE_PRINT("Initiating Shaders");
	gx::GXShaderManager::init(); GXE_INFO("Shader Manager initialized !");
	//preload models
	gx::io::IORequestHandler::importModel("res/models/sphere/", "spheres.obj");
	gx::io::IORequestHandler::waitTasks();
	gx::LayerManager::getInstance().init();//instantiate a layer manager.[first ref]
	GXE_INFO("Layers Manager Initialized !");

}


