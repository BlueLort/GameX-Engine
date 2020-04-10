#pragma once
#include "Config.h"
#include "pch.h"
namespace gx {
	class GX_DLL GXTimer {//Class to calculate delta time and ticks between frames using whatever windowing API
	public:
		void init() {
			lastTicks = SDL_GetTicks();
		}
		inline static GXTimer& getAppTimer() {
			static GXTimer gxt;
			return gxt;
		}
		void update() {
			GXuint32 currentTicks = SDL_GetTicks();
			deltaTicks = currentTicks - lastTicks;
			lastTicks = currentTicks;
		}
		inline GXuint32 getDeltaTicks()const { return deltaTicks; }

	private:
		GXuint32 deltaTicks;
		GXuint32 lastTicks;

	};
}