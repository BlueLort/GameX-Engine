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
				float amp = 1.25f;
				float curr = 0.0f;
				for (GXint32 j = 0; j < nOctaves; j++) {

					GXint32 X = i % width, Y = static_cast<GXint32>(i / width);
					curr += abs(PerlinNoise::noise(X / quality, Y / quality, zPlane) * quality * amp);
					quality *= scale/lacunarity;
					amp *= persistence;
				}
				if (curr > maxH)maxH = curr;
				else if (curr < minH)minH = curr;
				heightsNormalized[i] = curr;
				quality = 3.5f;
				amp = 1.25f;
				curr = 0.0f;
			}
			GXint32 k = 0;
			for (GXint32 y = 0; y < height; y++) {
				for (GXint32 x = 0; x < totalWidth; x += 3) {
					GXint32 loc = totalWidth * y + x;
					heightsNormalized[k] = GXMaths::inverseLerp(heightsNormalized[k], minH, maxH);
					heightsColor[loc] = heightsNormalized[k] * 255;
					heightsColor[loc + 1] = heightsColor[loc];
					heightsColor[loc + 2] = heightsColor[loc];
					k++;
				}
			}
			



		}
		NoiseGenerator(GXint32 Width, GXint32 Height, float Scale,
			GXint32 NOctaves, float Persistence, float Lacunarity, float z)
			: HeightMapGenerator(Width, Height, Scale),
			nOctaves(NOctaves), persistence(Persistence), lacunarity(Lacunarity), zPlane(z) {

		}

	private:

		GXint32 nOctaves;
		float persistence;
		float lacunarity;
		GXint32 zPlane;
		inline bool isValid(GXint32 x, GXint32 y, GXint32 totalWidth)
		{
			// True if ij are valid indices; false otherwise.
			return
				y >= 0 && y < height &&
				x >= 0 && x < totalWidth;
		}
		inline float average(GXint32 x, GXint32 y, GXint32 totalWidth)
		{
			float avg = 0.0f;
			GXint32 counter = 0;
			for (GXint32 i = y - 1; i <= y + 1; i++)
			{
				for (GXint32 j = x - 3; j <= x + 3; j += 3)
				{
					if (isValid(j, i, totalWidth))
					{
						avg += heightsNormalized[i * totalWidth + j];
						counter++;
					}
				}
			}
			return avg / counter;
		}




	};

}