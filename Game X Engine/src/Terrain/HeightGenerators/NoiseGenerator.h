#pragma once
#include "HeightMapGenerator.h"
#include "Maths/Random/PerlinNoise.h"
#include "IOManagement/IOManager.h"

namespace gx {

	class GX_DLL NoiseGenerator : public HeightMapGenerator {
	public:
		virtual inline  void init() override {
			float maxH = FLT_MIN;
			float minH = FLT_MAX;
			GXint32 totalWidth = 3 * width;
			GXint32 size = width * height;
			for (GXint32 i = 0; i < size; i++) {
				float quality = 3.5f;
				float amp = 1.45f;
				float curr = 0.0f;
				for (GXint32 j = 0; j < nOctaves; j++) {

					GXint32 X = i % width, Y = i / width;
					curr += static_cast<GXFloat>(abs(PerlinNoise::noise(X / quality, Y / quality, zPlane) * quality * amp));
					quality *= scale/lacunarity;
					amp *= persistence;
				}
				if (curr > maxH)maxH = curr;
				else if (curr < minH)minH = curr;
				heightsNormalized[i] = curr;
				quality = 3.5f;
				amp = 1.45f;
				curr = 0.0f;
			}
			GXint32 k = 0;
			for (GXint32 y = 0; y < height; y++) {
				for (GXint32 x = 0; x < totalWidth; x += 3) {
					GXint32 loc = totalWidth * y + x;
					heightsNormalized[k] = static_cast<GXFloat>(GXMaths::inverseLerp(heightsNormalized[k], minH, maxH));
					heightsColor[loc] = static_cast<GXuint8>(heightsNormalized[k] * 255.9f);
					heightsColor[loc + 1] = heightsColor[loc];
					heightsColor[loc + 2] = heightsColor[loc];
					k++;
				}
			}
			



		}
		GXFloat* getHeightsNormalized() { return heightsNormalized; }
		NoiseGenerator(GXint32 Width, GXint32 Height, float Scale,
			GXint32 NOctaves, float Persistence, float Lacunarity, float z)
			: HeightMapGenerator(Width, Height, Scale),
			nOctaves(NOctaves), persistence(Persistence), lacunarity(Lacunarity), zPlane(z) {
			heightsNormalized = new GXFloat[static_cast<GXint64>(Width) * Height];
		}

	private:
		GXFloat* heightsNormalized;
		GXint32 nOctaves;
		GXFloat persistence;
		GXFloat lacunarity;
		GXFloat zPlane;
	};

}