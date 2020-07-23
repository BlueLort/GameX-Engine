#pragma once
#include "HeightMapGenerator.h"
#include "IOManagement/IORequestHandler.h"

namespace gx {

	class GX_DLL TextureMapGenerator : public HeightMapGenerator {
	public:
		virtual inline  void init() override {
			if (HMapImg->data == nullptr)return;
			GXint64 size = static_cast<GXint64>(HMapImg->width) * HMapImg->height * 3;
			heightsColor= new uint8_t[size];

			GXint32 k = 0;
			for (GXint32 y = 0; y < HMapImg->height; y++) {
				for (GXint32 x = 0; x < HMapImg->width; x++) {
					uint16_t val = 0;
					for (GXint32 ite = 0; ite < HMapImg->nChannels; ite++) {
						val += HMapImg->data[(y * HMapImg->width + x)* HMapImg->nChannels + ite];
					}
					heightsColor[k] = val / static_cast<GXuint8>(HMapImg->nChannels);
					heightsColor[k + 1] = val / static_cast<GXuint8>(HMapImg->nChannels);
					heightsColor[k + 2] = val / static_cast<GXuint8>(HMapImg->nChannels);
					k+=3;
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

		}

	private:
		const char* filePath;
		std::shared_ptr<io::ImageData> HMapImg;
	};
}