#pragma once
#include "Config.h"

namespace gx {
	class GX_DLL HeightMapGenerator {
	public:
		HeightMapGenerator(GXint32 Width, GXint32 Height, float Scale)
			:width(Width), height(Height), scale(Scale)
		{
			heightsNormalized = new float[Width * Height];
			heightsColor = new uint8_t[Width * Height*3];
		}
		HeightMapGenerator() {}
		inline float getValueNormalized(GXint32 x, GXint32 y) const {
			return heightsNormalized[width * y + x];
		}
		inline float getValueColor(GXint32 x, GXint32 y) const {
			return heightsColor[width * y + x];
		}
		virtual inline void init() = 0;
		float* getHeightsNormalized()const { return heightsNormalized; }
		uint8_t* getHeightsColor()const { return heightsColor; }
		GXint32 getWidth()const { return width; }
		GXint32 getHeight()const { return height; }
	protected:
		float* heightsNormalized;
		uint8_t* heightsColor;
		GXint32 width;
		GXint32 height;
		float scale;
	};
}