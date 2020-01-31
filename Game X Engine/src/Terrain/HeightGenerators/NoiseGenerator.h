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
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					float freq = 1;
					float amp = 1;
					float curr = 0;
					for (int i = 0; i < nOctaves; i++) {
						float factor = freq / scale;
						float X = x * factor;
						float Y = y * factor;
						float Z = seed * factor;
						float val = PerlinNoise::noise(X, Y, Z);
						curr += val * amp;
						amp *= roughness;
						freq *= lacunarity;
					}
					if (curr > maxH)maxH = curr;
					else if (curr < minH)minH = curr;
					heights[width * y + x] = curr;
				}
			}
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					int loc = width * y + x;
					heights[loc] = GXMaths::inverseLerp(heights[loc], minH, maxH);
				}
			}

		}
		NoiseGenerator(int Width, int Height, float Scale,
			int NOctaves, float Roughness, float Lacunarity, int Seed)
			: HeightMapGenerator(Width, Height, Scale),
			nOctaves(NOctaves), roughness(Roughness), lacunarity(Lacunarity), seed(Seed) {

		}

	private:
		int nOctaves;
		float roughness;
		float lacunarity;
		int seed;

		




	};
}