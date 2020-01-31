#pragma once
#include "HeightMapGenerator.h"
#include "IOManagement/IOManager.h"

namespace gx {

	class GX_DLL TextureMapGenerator : public HeightMapGenerator {
	public:
		virtual inline  void init() override {


		}
		TextureMapGenerator(int Width, int Height, int Scale,
			const char* FilePath)
			: HeightMapGenerator(Width, Height, Scale), filePath(FilePath) {

		}

	private:
		const char* filePath;

	};
}