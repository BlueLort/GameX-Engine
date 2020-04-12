#pragma once
#include "pch.h"
#include "Config.h"
#include "Events/Event.h"
#include "Layers/LayerManager.h"
#include "UI/GXUserInterface.h"
#include "Renderer/Renderer.h"
#include "Input/InputManager.h"
#include "Object/GXMeshObject/GXModelObject.h"
#include "Camera/EditorCamera/EditorCamera.h"
#include "Terrain/HeightGenerators/NoiseGenerator.h"
#include "GXTimer.h"
#include "Terrain/Plane/Plane.h"
#include "Physics/PhysicsManager.h"
#define GXPRINTF(...) (gx::LayerManager::getInstance().GXPrintf(__VA_ARGS__));
namespace gx {
	//adapt it to whatever API im using.
	using GX_SDLEvent = SDL_Event;
	using GXEventCallBack = SDL_EventFilter;
#define GXPollEvents SDL_PollEvent

	class GX_DLL GameXApp
	{
	public:
		GameXApp();
		virtual ~GameXApp();
		void Start();
		GXEventCallBack getEventCallBack() const { return onEvent; }
		//static std::unique_ptr<Renderer> renderer;
		static std::unique_ptr<GXUserInterface> gxUserInterface;
	private:
		
		//EVENT HANDLING
		static GXint32 onEvent(void* userdata, GX_SDLEvent* Event);
		std::future<void> rayPickingTask;
		static void rayPicking();
		static bool mainSceneSelected;
		
		template<class T>
		static GXint32 dispatchSystemEvent(std::shared_ptr<T>& gxEvent,GXuint32 windowID);

		template<class T>
		inline static GXint32 handleEvent(std::shared_ptr<T>& Event) { return 0; }
		static bool isRunning;
		
		

	};
	extern GameXApp* CreateApp();

	template<class T>
	inline GXint32 GameXApp::dispatchSystemEvent(std::shared_ptr<T>& gxEvent,GXuint32 windowID)
	{
		//TODO PROFILE THIS ,OVERHEAD -> Change it !
		bool handled = false;
		//input manager can only manage main window inputs
		if (mainSceneSelected&&InputManager::getInstance().handleEvent(gxEvent)) {
			handled = true;
		}
		if (gxUserInterface->handleEvent(gxEvent)) {
			handled = true;
		}
		if (handleEvent(gxEvent)) {
			handled = true;
		}


		return handled;
	}


	template<>
	inline GXint32 GameXApp::handleEvent<gx::event::WindowCloseEvent>(std::shared_ptr<gx::event::WindowCloseEvent>& Event) {
		isRunning = false;
		Event->handled = true;
		return 1;
	}
	template<>
	inline GXint32 GameXApp::handleEvent<gx::event::WindowResizeEvent>(std::shared_ptr<gx::event::WindowResizeEvent>& Event) {
		GXWindow::windowData->width = Event->getWidth();
		GXWindow::windowData->height = Event->getHeight();
#ifdef USING_EDITOR_CAMERA
		EditorCamera::getInstance().AR = static_cast<float>(Event->getWidth()) / static_cast<float>(Event->getHeight());
		EditorCamera::getInstance().processMouseScroll(0);//recalculate the projection matrix
#endif
		
		

		Event->handled = true;
		return 1;
	}
	template<>
	inline GXint32 GameXApp::handleEvent<gx::event::WindowMinimizeEvent>(std::shared_ptr<gx::event::WindowMinimizeEvent>& Event) {
		Event->handled = true;
		return 1;
	}
	template<>
	inline GXint32 GameXApp::handleEvent<gx::event::WindowMaximizeEvent>(std::shared_ptr<gx::event::WindowMaximizeEvent>& Event) {
		Event->handled = true;
		return 1;
	}
	template<>
	inline GXint32 GameXApp::handleEvent<gx::event::MousePressEvent>(std::shared_ptr<gx::event::MousePressEvent>& Event) {
		LayerManager::getInstance().mousePressRequest();
		Event->handled = true;
		return 1;
	}
}

