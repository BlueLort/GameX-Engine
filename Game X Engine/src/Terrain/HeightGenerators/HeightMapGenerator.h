#pragma once
#include "Config.h"

namespace gx {
	class GX_DLL HeightMapGenerator {
	public:
		HeightMapGenerator(GXint32 Width, GXint32 Height, GXFloat Scale)
			:width(Width), height(Height), scale(Scale)
		{
			heightsNormalized = new GXFloat[Width * Height];
			heightsColor = new GXuint8[Width * Height*3];
		}
		HeightMapGenerator():height(256),width(256) {}
		inline GXFloat getValueNormalized(GXint32 x, GXint32 y) const {
			return heightsNormalized[width * y + x];
		}
		inline GXFloat getValueColor(GXint32 x, GXint32 y) const {
			return heightsColor[width * y + x];
		}
		virtual inline void init() = 0;
		GXFloat* getHeightsNormalized()const { return heightsNormalized; }
		uint8_t* getHeightsColor()const { return heightsColor; }
		GXint32 getWidth()const { return width; }
		GXint32 getHeight()const { return height; }
	protected:
		GXFloat* heightsNormalized;
		uint8_t* heightsColor;
		GXint32 width;
		GXint32 height;
		GXFloat scale;
	};
}