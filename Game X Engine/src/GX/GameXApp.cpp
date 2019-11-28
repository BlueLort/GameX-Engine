#include "pch.h"
#include "GameXApp.h"
#define DISPATCH_EVENT(ev,dispatched,name) std::shared_ptr<##ev> gxEvent = std::make_shared<##ev>();\
dispatched = dispatchSystemEvent<##ev>(gxEvent);\
eventName = gxEvent->getName()

#define DISPATCH_EVENT_D1(ev,data1,dispatched,name) std::shared_ptr<##ev> gxEvent = std::make_shared<##ev>(data1);\
dispatched = dispatchSystemEvent<##ev>(gxEvent);\
eventName = gxEvent->getName()

#define DISPATCH_EVENT_D2(ev,data1,data2,dispatched,name) std::shared_ptr<##ev> gxEvent = std::make_shared<##ev>(data1,data2);\
dispatched = dispatchSystemEvent<##ev>(gxEvent);\
eventName = gxEvent->getName()

#define DISPATCH_EVENT_D2_NODATA(ev,data1,data2) std::shared_ptr<##ev> gxEvent = std::make_shared<##ev>(data1,data2);\
dispatchSystemEvent<##ev>(gxEvent)

#define DISPATCH_EVENT_D3(ev,data1,data2,data3,dispatched,name) std::shared_ptr<##ev> gxEvent = std::make_shared<##ev>(data1,data2,data3);\
dispatched = dispatchSystemEvent<##ev>(gxEvent);\
eventName = gxEvent->getName()

#define DISPATCH_EVENT_D3_NODATA(ev,data1,data2,data3,dispatched,name) std::shared_ptr<##ev> gxEvent = std::make_shared<##ev>(data1,data2,data3);\
dispatchSystemEvent<##ev>(gxEvent)

namespace gx {
	std::unique_ptr<ImGUI_SDLGL> GameXApp::UI_GL = std::make_unique<ImGUI_SDLGL>("ImGUI Editor");
	bool GameXApp::isRunning = true;

	GameXApp::GameXApp() {

	}

	GameXApp::~GameXApp() {

	}
	void GameXApp::Start() {
		//Bugs in GXModelIOManager
		GXModelObject object;

		object.GLinit("res/models/icosphere/", "icosphere.obj");
	
		while (isRunning) {
			GXTimer::getAppTimer().update();
			InputManager::getInstance().update();
			while (GXPollEvents(&GX_SDLEvent()) == 1);//Send events to callback
			EditorCamera::getInstance().update();
			//Render
	#ifdef USING_OPENGL  
			GLRenderer::getInstance().begin();
			
			object.GLDraw();
		
			//ImGUI Rendering
			UI_GL->startFrame();
			UI_GL->onGUIRender();// for all layers
			UI_GL->render();
			UI_GL->endFrame();
			GLRenderer::getInstance().end();
	#endif
			

		}
	}

	int GameXApp::onEvent(void* userdata, GX_SDLEvent* Event)
	{
		//SDL Event Mapping 
		bool dispatched = false;
		std::string eventName;
		switch (Event->type) {
		case SDL_QUIT:
		{DISPATCH_EVENT_D1(gx::event::WindowCloseEvent, 0, dispatched, eventName); }
		break;
		case gx::event::GXEventClass::GX_APPLICATION:
			switch (Event->window.event) {
			case gx::event::GXEventType::GX_WINDOW_CLOSE:
			{DISPATCH_EVENT_D1(gx::event::WindowCloseEvent, Event->window.windowID, dispatched, eventName); }
			break;
			case gx::event::GXEventType::GX_WINDOW_RESIZE:
			{DISPATCH_EVENT_D3(gx::event::WindowResizeEvent, Event->window.data1, Event->window.data2, Event->window.windowID, dispatched, eventName); }
			break;
			case gx::event::GXEventType::GX_WINDOW_MINIMIZE:
			{DISPATCH_EVENT_D3(gx::event::WindowMinimizeEvent, Event->window.data1, Event->window.data2, Event->window.windowID, dispatched, eventName); }
			break;
			case gx::event::GXEventType::GX_WINDOW_MAXIMIZE:
			{DISPATCH_EVENT_D3(gx::event::WindowMaximizeEvent, Event->window.data1, Event->window.data2, Event->window.windowID, dispatched, eventName); }
			break;

			}
			break;
		case gx::event::GXEventType::GX_KEY_PRESSED:
		{DISPATCH_EVENT_D2(gx::event::KeyPressEvent, Event->key.keysym.scancode,Event->key.windowID, dispatched, eventName); }
		break;
		case gx::event::GXEventType::GX_KEY_RELEASED:
		{DISPATCH_EVENT_D2(gx::event::KeyReleaseEvent, Event->key.keysym.scancode,Event->key.windowID, dispatched, eventName); }
		break;
		case gx::event::GXEventType::GX_KEY_TYPED:
		{DISPATCH_EVENT_D2(gx::event::KeyTypedEvent, Event->text.text,Event->text.windowID, dispatched, eventName); }
		break;
		case gx::event::GXEventType::GX_MOUSE_MOVED:
		{DISPATCH_EVENT_D3(gx::event::MouseMoveEvent, Event->motion.x, Event->motion.y,Event->motion.windowID, dispatched, eventName); }
		{DISPATCH_EVENT_D3_NODATA(gx::event::MouseMoveRelEvent, Event->motion.xrel, Event->motion.yrel,Event->motion.windowID); }
		break;
		case gx::event::GXEventType::GX_MOUSE_PRESSED:
		{DISPATCH_EVENT_D2(gx::event::MousePressEvent, Event->button.button,Event->button.windowID, dispatched, eventName); }
		break;
		case gx::event::GXEventType::GX_MOUSE_RELEASED:
		{DISPATCH_EVENT_D2(gx::event::MouseReleaseEvent, Event->button.button,Event->button.windowID, dispatched, eventName); }
		break;
		case gx::event::GXEventType::GX_MOUSE_SCROLL:
		{DISPATCH_EVENT_D3(gx::event::MouseScrollEvent, Event->wheel.x, Event->wheel.y,Event->wheel.windowID, dispatched, eventName); }
		break;

		}

		//if (dispatched) {
		//	GXE_DEBUG("Event Handled : " + eventName);
		//}
		return dispatched;
	}



}