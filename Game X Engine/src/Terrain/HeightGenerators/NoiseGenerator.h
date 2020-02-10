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
			int size = totalWidth * height;
			for (int i = 0; i < size; i += 3) {
				float quality = 1.0f;
				float amp = 1.75;
				float curr = 0.0f;
				for (int j = 0; j < nOctaves; j++) {

					int X = i % totalWidth, Y = static_cast<int>(i / totalWidth);
					curr += abs(PerlinNoise::noise(X / quality, Y / quality, zPlane) * quality * amp);
					quality *= scale/lacunarity;
					amp *= persistence;
				}
				if (curr > maxH)maxH = curr;
				else if (curr < minH)minH = curr;
				heightsNormalized[i] = curr;
				quality = 1.0f;
				amp = 1.25f;
				curr = 0.0f;
			}

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < totalWidth; x += 3) {
					int loc = totalWidth * y + x;
					heightsNormalized[loc] = GXMaths::inverseLerp(heightsNormalized[loc], minH, maxH);
					heightsNormalized[loc + 1] = heightsNormalized[loc];
					heightsNormalized[loc + 2] = heightsNormalized[loc];
					heightsColor[loc] = heightsNormalized[loc] * 255;
					heightsColor[loc + 1] = heightsColor[loc];
					heightsColor[loc + 2] = heightsColor[loc];
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
		inline bool isValid(int x, int y, int totalWidth)
		{
			// True if ij are valid indices; false otherwise.
			return
				y >= 0 && y < height &&
				x >= 0 && x < totalWidth;
		}
		inline float average(int x, int y, int totalWidth)
		{
			float avg = 0.0f;
			int counter = 0;
			for (int i = y - 1; i <= y + 1; i++)
			{
				for (int j = x - 3; j <= x + 3; j += 3)
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