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
			int totalWidth = 3 * width;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < totalWidth; x+=3) {
					float freq = 1;
					float amp = 1;
					float curr = 0;
					for (int i = 0; i < nOctaves; i++) {
						float factor = freq / scale;
						float X = x * factor;
						float Y = y * factor;
						float Z = zPlane * factor;
						float val = PerlinNoise::noise(X, Y, Z);
						curr += val * amp;
						amp *= persistence;
						freq *= lacunarity;
					}
					if (curr > maxH)maxH = curr;
					else if (curr < minH)minH = curr;
					heightsNormalized[totalWidth * y + x] = curr;
				}
			}
			
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < totalWidth; x+=3) {
					int loc = totalWidth * y + x;
					heightsNormalized[loc] = GXMaths::inverseLerp(heightsNormalized[loc], minH, maxH);
					heightsNormalized[loc+1] = heightsNormalized[loc];
					heightsNormalized[loc+2] = heightsNormalized[loc];
					heightsColor[loc] = heightsNormalized[loc] * 255;
					heightsColor[loc+1] = heightsColor[loc];
					heightsColor[loc+2] = heightsColor[loc];
				}
			}

		}
		NoiseGenerator(int Width, int Height, float Scale,
			int NOctaves, float Persistence, float Lacunarity, float z)
			: HeightMapGenerator(Width, Height, Scale),
			nOctaves(NOctaves), persistence(Persistence), lacunarity(Lacunarity), zPlane(z) {

		}

	private:
		
		int nOctaves;
		float persistence;
		float lacunarity;
		int zPlane;

		




	};
}