#pragma once
#include "pch.h"
#include "Config.h"
#include "Window/Layer.h"
namespace gx {
	//adapt it to whatever API im using.
	using GXEvent = SDL_Event;
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


		std::unique_ptr<Timer> timer;
	private:
		static int onEvent(void* userdata, GXEvent* Event);
		bool isRunning;
	
	};

	extern GameXApp* CreateApp();

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

