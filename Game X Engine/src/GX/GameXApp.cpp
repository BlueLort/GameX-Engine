#include "pch.h"
#include "GameXApp.h"


//IGNORE EVENTS DEBUGGING DATA FOR NOW

#define DISPATCH_EVENT(ev,windowID,...) std::shared_ptr<##ev> gxEvent = std::make_shared<##ev>(__VA_ARGS__);\
 dispatchSystemEvent<##ev>(gxEvent,windowID)

namespace gx {
	std::unique_ptr<GXUserInterface> GameXApp::gxUserInterface = std::make_unique<GXUserInterface>("ImGUI Editor");
	bool GameXApp::isRunning = true;
	bool GameXApp::mainSceneSelected = false;


	GameXApp::GameXApp() {

	}

	GameXApp::~GameXApp() {

	}

	void GameXApp::Start() {
		//TODO do RAY PICKING WITH PHYSICS ENGINE ?
		//rayPickingTask = std::async(std::launch::async, rayPicking);
		while (isRunning) {
			GXFloat deltaTime = 1.0f / GXTimer::getAppTimer().getDeltaTicks();
			GXTimer::getAppTimer().update();
			InputManager::getInstance().update();
			while (GXPollEvents(&GX_SDLEvent()) == 1);//Send events to callback
			EditorCamera::getInstance().update();
			mainSceneSelected = LayerManager::getInstance().isMainSceneSelected();
			io::IORequestHandler::update();//check if something is imported and init it using openGL context
			GXPhysicsManager::getInstance().stepSimulation(deltaTime);
			//Render
			LayerManager::getInstance().renderUpdateLayers(1.0f / GXTimer::getAppTimer().getDeltaTicks());
			//ImGUI Rendering
			gxUserInterface->startFrame();
			GXGizmo::begin(); // gizmo begin frame
			gxUserInterface->onGUIRender();
			LayerManager::getInstance().onGUIRender();
			gxUserInterface->render();
			gxUserInterface->endFrame();

			GXWindow::swapWindow();


		}
		io::IORequestHandler::destroy();//destroy models and wait all tasks to finish.
	}

	GXint32 GameXApp::onEvent(void* userdata, GX_SDLEvent* Event)
	{
		//SDL Event Mapping 
		bool dispatched = false;
		std::string eventName;
		switch (Event->type) {
		case SDL_QUIT:
		{DISPATCH_EVENT(gx::event::WindowCloseEvent, Event->window.windowID, 0); }
		break;
		case gx::event::GXEventClass::GX_APPLICATION:
			switch (Event->window.event) {
			case gx::event::GXEventType::GX_WINDOW_CLOSE:
			{DISPATCH_EVENT(gx::event::WindowCloseEvent, Event->window.windowID, Event->window.windowID); }
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
		{DISPATCH_EVENT(gx::event::KeyPressEvent, Event->key.windowID, Event->key.keysym.scancode, Event->key.windowID); }
		break;
		case gx::event::GXEventType::GX_KEY_RELEASED:
		{DISPATCH_EVENT(gx::event::KeyReleaseEvent, Event->key.windowID, Event->key.keysym.scancode, Event->key.windowID); }
		break;
		case gx::event::GXEventType::GX_KEY_TYPED:
		{DISPATCH_EVENT(gx::event::KeyTypedEvent, Event->text.windowID, Event->text.text, Event->text.windowID); }
		break;
		case gx::event::GXEventType::GX_MOUSE_MOVED:
		{DISPATCH_EVENT(gx::event::MouseMoveEvent, Event->motion.windowID, Event->motion.x, Event->motion.y, Event->motion.windowID); }
		{DISPATCH_EVENT(gx::event::MouseMoveRelEvent, Event->motion.windowID, Event->motion.xrel, Event->motion.yrel, Event->motion.windowID); }
		break;
		case gx::event::GXEventType::GX_MOUSE_PRESSED:
		{DISPATCH_EVENT(gx::event::MousePressEvent, Event->button.windowID, Event->button.button, Event->button.windowID); }
		break;
		case gx::event::GXEventType::GX_MOUSE_RELEASED:
		{DISPATCH_EVENT(gx::event::MouseReleaseEvent, Event->button.windowID, Event->button.button, Event->button.windowID); }
		break;
		case gx::event::GXEventType::GX_MOUSE_SCROLL:
		{DISPATCH_EVENT(gx::event::MouseScrollEvent, Event->wheel.windowID, Event->wheel.x, Event->wheel.y, Event->wheel.windowID); }
		break;

		}
		return dispatched;
	}

	void GameXApp::rayPicking()
	{
		while (isRunning) {
			if (InputManager::getInstance().isPressed(event::key::GXK_MOUSE_LEFT)) {
				std::pair<GXFloat, GXFloat> ml = LayerManager::getInstance().getSceneMouseLocNormalized();
				std::string message = "";
				
				if (ml.first <= 1.0f && ml.first >= 0.0f && ml.second <= 1.0f && ml.second >= 0) {
					ml.first = (ml.first - 0.5f) * 2.0f;
					ml.second = (ml.second - 0.5f) * 2.0f;
					GXVec4 rayStartNDC(ml.first, ml.second, -1.0, 1.0f);
					GXVec4 rayEndNDC(ml.first, ml.second, 0.0f, 1.0f);

					GXMat4 invProjView = GXMaths::inverse(EditorCamera::getInstance().getPVMatrix());
					GXVec4 rayStartWorld = invProjView * rayStartNDC;
					rayStartWorld /= rayStartWorld.w;
					GXVec4 rayEndWorld = invProjView * rayEndNDC;
					rayEndWorld /= rayEndWorld.w;

					GXVec3 rayDirWorld=GXMaths::normalize(rayEndWorld - rayStartWorld);

					GXVec3 rayOrigin = EditorCamera::getInstance().transform.position;
					GXVec3 rayEnd = rayOrigin + rayDirWorld * 1000.0f;
					btVector3 btRayStart(rayOrigin.x, rayOrigin.y, rayOrigin.z);
					btVector3 btRayEnd(rayEnd.x, rayEnd.y, rayEnd.z);
					btCollisionWorld::ClosestRayResultCallback rayCallback(btRayStart, btRayEnd);
					GXPhysicsManager::getInstance().rayTest(btRayStart, btRayEnd,rayCallback);
					if (rayCallback.hasHit()) {
						std::ostringstream oss;
						oss << LayerManager::getInstance().getModelObject(reinterpret_cast<GXuint32>(rayCallback.m_collisionObject->getUserPointer()))->getName();
						message = oss.str();
					}
					else {
						message = "background";
					}

					GXPRINTF("Ray Dir World x= %.3f y= %.3f z=%.3f , message=%s", rayDirWorld.x, rayDirWorld.y, rayDirWorld.z, message.c_str());
					
				}
				while (InputManager::getInstance().isPressed(event::key::GXK_MOUSE_LEFT));
			}
		}
	}



}