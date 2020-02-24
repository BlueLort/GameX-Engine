#include "pch.h"
#include "GameXApp.h"


//IGNORE EVENTS DEBUGGING DATA FOR NOW

#define DISPATCH_EVENT(ev,windowID,...) std::shared_ptr<##ev> gxEvent = std::make_shared<##ev>(__VA_ARGS__);\
 dispatchSystemEvent<##ev>(gxEvent,windowID)

namespace gx {
	std::unique_ptr<ImGUI_SDLGL> GameXApp::UI_GL = std::make_unique<ImGUI_SDLGL>("ImGUI Editor");
	bool GameXApp::isRunning = true;
	bool GameXApp::mainSceneSelected = false;


	GameXApp::GameXApp() {

	}

	GameXApp::~GameXApp() {

	}

	void GameXApp::Start() {
		//OBJECT FOR DEBUGGING
		std::shared_ptr<GXModelObject> object = std::make_shared<GXModelObject>();
		object->GLinit("res/models/nanosuit/nanosuit.obj");
		LayerManager::getInstance().addModelObject(object);
		rayPickingTask = std::async(std::launch::async, rayPicking);
		while (isRunning) {
			float deltaTime = 1.0f / GXTimer::getAppTimer().getDeltaTicks();
			GXTimer::getAppTimer().update();
			InputManager::getInstance().update();
			while (GXPollEvents(&GX_SDLEvent()) == 1);//Send events to callback
			EditorCamera::getInstance().update();
			mainSceneSelected= LayerManager::getInstance().isMainSceneSelected();
			io::IORequestHandler::update();//check if something is imported and init it using openGL context
			GXPhysicsManager::getInstance().stepSimulation(deltaTime);
			//Render
	#ifdef USING_OPENGL  
			LayerManager::getInstance().renderUpdateLayers(1.0f/GXTimer::getAppTimer().getDeltaTicks());
			//ImGUI Rendering
			UI_GL->startFrame();
			UI_GL->onGUIRender();
			LayerManager::getInstance().onGUIRender();
			UI_GL->render();
			UI_GL->endFrame();
		
			GXWindow::swapWindow_GL();
	#endif
			

		}
		io::IORequestHandler::destroy();//destroy models and wait all tasks to finish.
	}

	int GameXApp::onEvent(void* userdata, GX_SDLEvent* Event)
	{
		//SDL Event Mapping 
		bool dispatched = false;
		std::string eventName;
		switch (Event->type) {
		case SDL_QUIT:
		{DISPATCH_EVENT(gx::event::WindowCloseEvent,Event->window.windowID, 0);}
		break;
		case gx::event::GXEventClass::GX_APPLICATION:
			switch (Event->window.event) {
			case gx::event::GXEventType::GX_WINDOW_CLOSE:
			{DISPATCH_EVENT(gx::event::WindowCloseEvent, Event->window.windowID,Event->window.windowID); }
			break;
			case gx::event::GXEventType::GX_WINDOW_RESIZE:
			{DISPATCH_EVENT(gx::event::WindowResizeEvent, Event->window.windowID, Event->window.data1, Event->window.data2, Event->window.windowID); }
			break;
			case gx::event::GXEventType::GX_WINDOW_MINIMIZE:
			{DISPATCH_EVENT(gx::event::WindowMinimizeEvent, Event->window.windowID, Event->window.data1, Event->window.data2, Event->window.windowID); }
			break;
			case gx::event::GXEventType::GX_WINDOW_MAXIMIZE:
			{DISPATCH_EVENT(gx::event::WindowMaximizeEvent, Event->window.windowID, Event->window.data1, Event->window.data2, Event->window.windowID); }
			break;
			}
			break;
		case gx::event::GXEventType::GX_KEY_PRESSED:
		{DISPATCH_EVENT(gx::event::KeyPressEvent, Event->key.windowID, Event->key.keysym.scancode,Event->key.windowID); }
		break;
		case gx::event::GXEventType::GX_KEY_RELEASED:
		{DISPATCH_EVENT(gx::event::KeyReleaseEvent, Event->key.windowID, Event->key.keysym.scancode,Event->key.windowID); }
		break;
		case gx::event::GXEventType::GX_KEY_TYPED:
		{DISPATCH_EVENT(gx::event::KeyTypedEvent, Event->text.windowID, Event->text.text,Event->text.windowID); }
		break;
		case gx::event::GXEventType::GX_MOUSE_MOVED:
		{DISPATCH_EVENT(gx::event::MouseMoveEvent, Event->motion.windowID, Event->motion.x, Event->motion.y,Event->motion.windowID); }
		{DISPATCH_EVENT(gx::event::MouseMoveRelEvent, Event->motion.windowID, Event->motion.xrel, Event->motion.yrel,Event->motion.windowID); }
		break;
		case gx::event::GXEventType::GX_MOUSE_PRESSED:
		{DISPATCH_EVENT(gx::event::MousePressEvent, Event->button.windowID, Event->button.button,Event->button.windowID); }
		break;
		case gx::event::GXEventType::GX_MOUSE_RELEASED:
		{DISPATCH_EVENT(gx::event::MouseReleaseEvent, Event->button.windowID, Event->button.button,Event->button.windowID); }
		break;
		case gx::event::GXEventType::GX_MOUSE_SCROLL:
		{DISPATCH_EVENT(gx::event::MouseScrollEvent, Event->wheel.windowID, Event->wheel.x, Event->wheel.y,Event->wheel.windowID); }
		break;

		}
		return dispatched;
	}

	void GameXApp::rayPicking()
	{
		while (isRunning) {
			if (InputManager::getInstance().isPressed(event::key::GXK_MOUSE_LEFT)) {
				std::pair<int32_t, int32_t> mouseLoc = InputManager::getInstance().getMouseLoc();
				GXE_DEBUG("mouse loc x= {0} y= {1}", mouseLoc.first, mouseLoc.second);
				std::pair<float, float> ml=LayerManager::getInstance().getSceneMouseLocNormalized();
				
				GXE_DEBUG("ImGui mouse loc x= {0} y= {1}", ml.first, ml.second);
				while (InputManager::getInstance().isPressed(event::key::GXK_MOUSE_LEFT));
			}
		}
	}



}