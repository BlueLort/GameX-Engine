#pragma once
#include "Config.h"

namespace gx {
	class GX_DLL HeightMapGenerator {
	public:
		HeightMapGenerator(int Width, int Height, float Scale)
			:width(Width), height(Height), scale(Scale)
		{
			heightsNormalized = new float[Width * Height*3];
			heightsColor = new uint8_t[Width * Height*3];
		}
		HeightMapGenerator() {}
		inline float getValueNormalized(int x, int y) const {
			return heightsNormalized[width * y + x];
		}
		inline float getValueColor(int x, int y) const {
			return heightsColor[width * y + x];
		}
		virtual inline void init() = 0;
		float* getHeightsNormalized()const { return heightsNormalized; }
		uint8_t* getHeightsColor()const { return heightsColor; }
		int getWidth()const { return width; }
		int getHeight()const { return height; }
	protected:
		float* heightsNormalized;
		uint8_t* heightsColor;
		int width;
		int height;
		float scale;
	};
}