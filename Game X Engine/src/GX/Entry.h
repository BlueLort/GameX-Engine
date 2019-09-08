#pragma once

inline void initSystems();
int main(int argc, char** argv) {
	initSystems();
	gx::GameXApp* Game = gx::CreateApp();
	Game->Start();
	delete Game;

	return 0;
}
constexpr uint32_t WINDOW_WIDTH = 1024;
constexpr uint32_t WINDOW_HEIGHT = 768;
constexpr uint32_t SDL_FLAGS = SDL_WINDOW_OPENGL;
//init different components of the engine
void initSystems() {
	gx::Logger::init();	GXE_INFO("Logger initialized successfully !");
	gx::GXWindow::init("Game X Engine", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_FLAGS); GXE_INFO("Window initialized successfully using SDL2.0 !");

}


