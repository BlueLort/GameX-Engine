#include <pch.h>
#include "GameXApp.h"

namespace gx {

	GameXApp::GameXApp() :isRunning(true) {

	}

	GameXApp::~GameXApp() {

	}

	void GameXApp::Start() {
		while (isRunning) {
			while(GXPollEvents(&GXEvent()));
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