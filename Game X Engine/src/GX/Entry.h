#pragma once
extern gx::GameX* gx::CreateApp();


int main(int argc, char** argv) {

	gx::Logger::init();
	
	gx::GameX* Game = gx::CreateApp();
	Game->Start();
	delete Game;

	return 0;
}



