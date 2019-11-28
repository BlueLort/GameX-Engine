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