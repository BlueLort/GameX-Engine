#pragma once
#include <pch.h>
#include <Config.h>

namespace gx {
	//adapt it to whatever API im using.
	using GXEvent = SDL_Event;
	using GXEventCallBack = SDL_EventFilter;
	#define GXPollEvents SDL_PollEvent
	class GX_DLL GameXApp
	{
	public:
		GameXApp();

		virtual ~GameXApp();
		void Start();
		GXEventCallBack getEventCallBack() const { return onEvent; }
		
	private:
		static int onEvent(void* userdata, GXEvent* Event);
		bool isRunning;
	};
	extern GameXApp* CreateApp();

}

