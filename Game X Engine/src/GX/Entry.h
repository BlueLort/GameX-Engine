#pragma once
extern gx::GameXApp* gx::CreateApp();


int main(int argc, char** argv) {

	gx::Logger::init();
	
	gx::GameXApp* Game = gx::CreateApp();
	Game->Start();
	delete Game;

	return 0;
}



