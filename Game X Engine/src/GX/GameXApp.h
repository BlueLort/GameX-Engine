#pragma once
#include "pch.h"
#include "Config.h"
#include "Events/Event.h"
#include "Layers/LayerManager.h"
#include "UI/ImGUI_SDLGL.h"
#include "Renderer/Renderer.h"
#include "Input/InputManager.h"
#include "Object/GXMeshObject/GXModelObject.h"
#include "Camera/EditorCamera/EditorCamera.h"
#include "GXTimer.h"
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
		static std::unique_ptr<ImGUI_SDLGL> UI_GL;
	private:
		//EVENT HANDLING
		static int onEvent(void* userdata, GX_SDLEvent* Event);

		static bool mainSceneSelected;

		template<class T>
		static int dispatchSystemEvent(std::shared_ptr<T>& gxEvent,uint32_t windowID);

		template<class T>
		inline static int handleEvent(std::shared_ptr<T>& Event) { return 0; }
		static bool isRunning;
		
		

	};
	extern GameXApp* CreateApp();

	template<class T>
	inline int GameXApp::dispatchSystemEvent(std::shared_ptr<T>& gxEvent,uint32_t windowID)
	{
		//TODO PROFILE THIS ,OVERHEAD -> Change it !
		bool handled = false;
		//input manager can only manage main window inputs
		if (mainSceneSelected&&mainSceneSelected&&InputManager::getInstance().handleEvent(gxEvent)) {
			handled = true;
		}
		if (UI_GL->handleEvent(gxEvent)) {
			handled = true;
		}

		if (handleEvent(gxEvent)) {
			handled = true;
		}


		return handled;
	}


	template<>
	inline int GameXApp::handleEvent<gx::event::WindowCloseEvent>(std::shared_ptr<gx::event::WindowCloseEvent>& Event) {
		isRunning = false;
		Event->handled = true;
		return 1;
	}
	template<>
	inline int GameXApp::handleEvent<gx::event::WindowResizeEvent>(std::shared_ptr<gx::event::WindowResizeEvent>& Event) {
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
	inline int GameXApp::handleEvent<gx::event::WindowMinimizeEvent>(std::shared_ptr<gx::event::WindowMinimizeEvent>& Event) {
		Event->handled = true;
		return 1;
	}
	template<>
	inline int GameXApp::handleEvent<gx::event::WindowMaximizeEvent>(std::shared_ptr<gx::event::WindowMaximizeEvent>& Event) {
		Event->handled = true;
		return 1;
	}

}

