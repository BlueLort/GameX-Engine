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
constexpr uint32_t SDL_FLAGS = SDL_WINDOW_OPENGL;
//init different components of the engine
void initSystems(gx::GameXApp* gameInst) {
	gx::Logger::init();	GXE_INFO("Logger initialized successfully !");
	gx::GXWindow::init("Game X Engine", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_FLAGS); GXE_INFO("Window initialized successfully using SDL2.0 !");
	gx::GXWindow::addEventsCallBack(gameInst->getEventCallBack(),nullptr);
}


