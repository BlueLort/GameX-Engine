#include "pch.h"
#include "GameXApp.h"

namespace gx {
	std::unique_ptr<ImGUI_SDLGL> GameXApp::ImGUIGL_Layer = std::make_unique<ImGUI_SDLGL>("ImGUI Editor");
	std::unique_ptr<Timer> GameXApp::timer = std::make_unique<Timer>();
	bool GameXApp::isRunning = true;

	GameXApp::GameXApp() {

	}

	GameXApp::~GameXApp() {

	}

	void GameXApp::Start() {
		while (isRunning) {
			timer->update();
			while (GXPollEvents(&GX_SDLEvent()) == 1);//Send events to callback
		}
	}

	int GameXApp::onEvent(void* userdata, GX_SDLEvent* Event)
	{
		std::shared_ptr<gx::event::GXEvent> gxEvent = nullptr;
		//SDL Event Mapping 
		switch (Event->type) {
		case gx::event::GXEventClass::GX_APPLICATION:
			switch (Event->window.event) {
			case gx::event::GXEventType::GX_WINDOW_CLOSE:
				gxEvent = std::make_shared<gx::event::WindowCloseEvent>();
				break;
			case gx::event::GXEventType::GX_WINDOW_RESIZE:
				gxEvent = std::make_shared<gx::event::WindowResizeEvent>(Event->window.data1, Event->window.data2);
				break;
			case gx::event::GXEventType::GX_WINDOW_MINIMIZE:
				gxEvent = std::make_shared<gx::event::WindowMinimizeEvent>(Event->window.data1, Event->window.data2);
				break;
			case gx::event::GXEventType::GX_WINDOW_MAXIMIZE:
				gxEvent = std::make_shared<gx::event::WindowMaximizeEvent>(Event->window.data1, Event->window.data2);
				break;
			case gx::event::GXEventType::GX_WINDOW_SIZE_CHANGED:
				//gxEvent = std::make_shared<gx::event::WindowSizeEvent>(Event->window.data1, Event->window.data2);
				break;
			}
			break;
		case gx::event::GXEventType::GX_KEY_PRESSED:

			break;
		case gx::event::GXEventType::GX_KEY_RELEASED:

			break;
		case gx::event::GXEventType::GX_MOUSE_MOVED:

			break;
		case gx::event::GXEventType::GX_MOUSE_PRESSED:

			break;
		case gx::event::GXEventType::GX_MOUSE_RELEASED:

			break;
		case gx::event::GXEventType::GX_MOUSE_SCROLL:

			break;
		}
		if (gxEvent != nullptr) {
			return dispatchEvent(gxEvent);
		}
		return 0;
	}

	int GameXApp::dispatchEvent(std::shared_ptr<gx::event::GXEvent>& gxEvent)
	{
		if (gxEvent->getEventType() == gx::event::GX_WINDOW_CLOSE) {
			isRunning = false;
			gxEvent->handled = true;
			return 1;
		}
		//TODO SEND EVENT TO ALL LAYERS IN A STACK PRIORITY MANNER UNTIL IT'S HANDLED OR TERMINATED

		return 0;
	}

}