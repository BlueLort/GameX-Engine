#pragma once
#include "Config.h"

namespace gx {
	class GX_DLL HeightMapGenerator {
	public:
		HeightMapGenerator(GXint32 Width, GXint32 Height, GXFloat Scale)
			:width(Width), height(Height), scale(Scale)
		{
			heightsColor = new GXuint8[static_cast<GXint64>(Width) * Height*3];
		}
		HeightMapGenerator():height(256),width(256), heightsColor(nullptr), scale(1){}
		inline GXFloat getValueColor(GXint32 x, GXint32 y) const {
			return heightsColor[width * y + x];
		}
		virtual inline void init() = 0;
		GXuint8* getHeightsColor()const { return heightsColor; }
		GXint32 getWidth()const { return width; }
		GXint32 getHeight()const { return height; }
	protected:
		GXuint8* heightsColor;
		GXint32 width;
		GXint32 height;
		GXFloat scale;
	};
}