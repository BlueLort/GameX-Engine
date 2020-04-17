#pragma once
#include "HeightMapGenerator.h"
#include "IOManagement/IORequestHandler.h"

namespace gx {

	class GX_DLL TextureMapGenerator : public HeightMapGenerator {
	public:
		virtual inline  void init() override {
			if (HMapImg->data == nullptr)return;
			GXint64 size = HMapImg->width * HMapImg->height * 3;
			heightsColor= new uint8_t[size];
			heightsNormalized = new GXFloat[size];

			GXint32 k = 0;
			GXint32 l = 0;
			for (GXint32 y = 0; y < HMapImg->height; y++) {
				for (GXint32 x = 0; x < HMapImg->width; x++) {
					uint16_t val = 0;
					for (GXint32 ite = 0; ite < HMapImg->nChannels; ite++) {
						val += HMapImg->data[y * HMapImg->width + x + ite];
					}
					heightsColor[k] = val / HMapImg->nChannels;
					heightsColor[k + 1] = heightsColor[k];
					heightsColor[k + 2] = heightsColor[k];
					heightsNormalized[l] = static_cast<GXFloat>(heightsColor[k] / 255.0f);
					k+=3;
					l++;
				}
			}
		}
		TextureMapGenerator(
			const char* FilePath)
			:filePath(FilePath) {

			 HMapImg = io::IORequestHandler::getImage(filePath, GX_HEIGHT);

		
			width = HMapImg->width;
			height = HMapImg->height;
			heightsColor = nullptr;
			heightsNormalized = nullptr;

		}

	private:
		const char* filePath;
		std::shared_ptr<io::ImageData> HMapImg;
	};
}