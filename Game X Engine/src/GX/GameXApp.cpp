#include "pch.h"
#include "GameXApp.h"

namespace gx {

	GameXApp::GameXApp() :isRunning(true),timer(std::make_unique<Timer>())
		,ImGUI_Layer(std::make_unique<ImGUI_SDLGL>("ImGUI Editor")){

	}

	GameXApp::~GameXApp() {

	}

	void GameXApp::Start() {
		while (isRunning) {
			timer->update();
			while (GXPollEvents(&GXEvent()) == 1);//Send events to callback
		}
	}

	int GameXApp::onEvent(void* userdata, GXEvent* Event)
	{
		//SDL Event Mapping 
		switch (Event->type) {



		}
		return 0;
	}

}