#pragma once
#include "Config.h"

namespace gx {
	class GX_DLL HeightMapGenerator {
	public:
		HeightMapGenerator(int Width, int Height, float Scale)
			:width(Width), height(Height), scale(Scale)
		{
			heights = new float[Width * Height];
		}
		inline double getValue(int x, int y) const {
			return heights[width * y + x];
		}
		virtual inline void init() = 0;
		virtual void destroy() { delete[] heights; }
		float* getHeightValues()const { return heights; }
		int getWidth()const { return width; }
		int getHeight()const { return height; }
	protected:
		float* heights;
		int width;
		int height;
		float scale;
	};
}