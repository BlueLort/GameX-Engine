#pragma once
#include "pch.h"
#include "Config.h"
#include "Events/Event.h"
#include "Window/Layer.h"
#include "UI/ImGUI_SDLGL.h"
#include "Renderer/Renderer.h"
namespace gx {
	//adapt it to whatever API im using.
	using GX_SDLEvent = SDL_Event;
	using GXEventCallBack = SDL_EventFilter;
#define GXPollEvents SDL_PollEvent
	class Timer;
	class GX_DLL GameXApp
	{
	public:
		GameXApp();
		virtual ~GameXApp();
		void Start();
		GXEventCallBack getEventCallBack() const { return onEvent; }
		//static std::unique_ptr<Renderer> renderer;
		static std::unique_ptr<ImGUI_SDLGL> UI_GL;
		static std::unique_ptr<Timer> timer;
	private:
		//EVENT HANDLING
		static int onEvent(void* userdata, GX_SDLEvent* Event);
		template<class T>
		static int dispatchSystemEvent(std::shared_ptr<T>& gxEvent);

		template<class T>
		static int handleEvent(std::shared_ptr<T>& Event) { return 0; }
		template<>
		static int handleEvent<gx::event::WindowCloseEvent>(std::shared_ptr<gx::event::WindowCloseEvent>& Event);
		template<>
		static int handleEvent<gx::event::WindowResizeEvent>(std::shared_ptr<gx::event::WindowResizeEvent>& Event);
		template<>
		static int handleEvent<gx::event::WindowMinimizeEvent>(std::shared_ptr<gx::event::WindowMinimizeEvent>& Event);
		template<>
		static int handleEvent<gx::event::WindowMaximizeEvent>(std::shared_ptr<gx::event::WindowMaximizeEvent>& Event);
		static bool isRunning;

	};
	extern GameXApp* CreateApp();

	template<class T>
	inline int GameXApp::dispatchSystemEvent(std::shared_ptr<T>& gxEvent)
	{
		bool handled = false;
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
		Renderer::setViewPort(Event->getWidth(), Event->getHeight());
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


	class GX_DLL Timer {//Class to calculate delta time and ticks between frames using whatever windowing API
	public:
		void init() {
			lastTicks = SDL_GetTicks();
		}
		void update() {
			uint32_t currentTicks = SDL_GetTicks();
			deltaTicks = currentTicks - lastTicks;
			lastTicks = currentTicks;
		}
		inline uint32_t getDeltaTicks()const { return deltaTicks; }

	private:
		uint32_t deltaTicks;
		uint32_t lastTicks;

	};


}

