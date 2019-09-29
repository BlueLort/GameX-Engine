#include "pch.h"
#include "GameXApp.h"

namespace gx {
	std::unique_ptr<ImGUI_SDLGL> GameXApp::ImGUIGL_Layer = std::make_unique<ImGUI_SDLGL>("ImGUI Editor");
	//std::unique_ptr<Renderer> GameXApp::renderer = std::make_unique<Renderer>();
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
			
			//Render
			Renderer::begin();


			ImGUIGL_Layer->startFrame();
			ImGUIGL_Layer->onGUIRender();// for all layers
			ImGUIGL_Layer->render();
			ImGUIGL_Layer->endFrame();

			Renderer::end();

		}
	}

	int GameXApp::onEvent(void* userdata, GX_SDLEvent* Event)
	{
		//SDL Event Mapping 
		bool dispatched = false;
		std::string eventName;
		switch (Event->type) {
		case gx::event::GXEventClass::GX_APPLICATION:
			switch (Event->window.event) {
				case gx::event::GXEventType::GX_WINDOW_CLOSE:
				{
					std::shared_ptr<gx::event::WindowCloseEvent> ev = std::make_shared<gx::event::WindowCloseEvent>();
					dispatched = dispatchEvent<gx::event::WindowCloseEvent>(ev);
					eventName = ev->getName();
					break;
				}
				case gx::event::GXEventType::GX_WINDOW_RESIZE:
				{
					std::shared_ptr<gx::event::WindowResizeEvent> ev = std::make_shared<gx::event::WindowResizeEvent>(Event->window.data1,Event->window.data2);
					dispatched = dispatchEvent<gx::event::WindowResizeEvent>(ev);
					eventName = ev->getName();
					break;
				}
				case gx::event::GXEventType::GX_WINDOW_MINIMIZE:
				{
					std::shared_ptr<gx::event::WindowMinimizeEvent> ev = std::make_shared<gx::event::WindowMinimizeEvent>(Event->window.data1, Event->window.data2);
					dispatched = dispatchEvent<gx::event::WindowMinimizeEvent>(ev);
					eventName = ev->getName();
					break;
				}
				case gx::event::GXEventType::GX_WINDOW_MAXIMIZE:
				{
					std::shared_ptr<gx::event::WindowMaximizeEvent> ev = std::make_shared<gx::event::WindowMaximizeEvent>(Event->window.data1, Event->window.data2);
					dispatched = dispatchEvent<gx::event::WindowMaximizeEvent>(ev);
					eventName = ev->getName();
					break;
				}
			}
			break;
			//TODO Create Event Instances for appropriate event with correct data
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
		//if (dispatched) {
		//	GXE_DEBUG("Event Handled : " + eventName);
		//}
		return dispatched;
	}

	

}