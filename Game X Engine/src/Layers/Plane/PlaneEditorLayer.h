#pragma once
#include "Config.h"
#include "Layers/Utility/NoiseGeneratorLayer.h"
#include "Terrain/Plane/Plane.h"
#include "Layers/Layer.h"
#include "Layers/Utility/NoiseGeneratorLayer.h"
#include "Layers/Main/MainScene.h"
/*
std::shared_ptr<GXPlane> plane = std::make_shared<GXPlane>(128, 128);
plane->init(LayerManager::getInstance().getHeightsNormalized());
plane->isReady = true;
LayerManager::getInstance().setPlane(plane);
*/

namespace gx {
	struct LevelData {
		GXFloat colors[3]; // tweaked through GUI and also used to represent 0 to 255 value color
		GXFloat heightVal;
		inline LevelData() :heightVal(-0.0001f) {
			colors[0] = 0.0f;
			colors[1] = 0.0f;
			colors[2] = 0.0f;
		}
	};
	class GX_DLL PlaneEditorLayer :public Layer {
	public:
		inline PlaneEditorLayer(const std::string& layerName) :
			Layer(layerName)
			, width(256), depth(256),height(16), texID(0), clusteringFactorX(2), clusteringFactorZ(2){
			levels.push_back(LevelData());
		}
		virtual void init()override;
		virtual void destroy()override;
		virtual void start()override;
		virtual void end()override;
		virtual GXint32 onEvent(const gx::event::GXEventType& eventType)override;
		virtual void onUpdate(GXFloat deltaTime)override;
		virtual void onGUIRender()override;

	private:
		ImGuiWindowFlags windowFlags;
		GXuint32 texID;
		static uint8_t* texColors;
		static std::shared_ptr<GXPlane> plane;
		static GXBool isUpdated;
		static GXBool canUpdate;
		GXint32 width;
		GXint32 depth;
		GXint32 height;
		GXint32 clusteringFactorX;
		GXint32 clusteringFactorZ;
		std::vector<LevelData> levels;
		std::future<void> asyncTask;

		static inline void createColoredTexture(GXint32 width, GXint32 depth, GXint32 height, GXint32 clusteringFactorX, GXint32 clusteringFactorZ, std::vector<LevelData> levels) {
			
			if (NoiseGeneratorLayer::heightsNormalized == nullptr|| !NoiseGeneratorLayer::canUpdate ) {
				canUpdate = true;
				return;
			}
			//levels sent by copy so i can do whatever i want on them
			std::sort(levels.begin(), levels.end(), [](LevelData& l1, LevelData& l2) {
				return l1.heightVal > l2.heightVal;
				});
			GXint32 nLevels = static_cast<GXint32>(levels.size());
			nLevels--;
			for (GXint32 i = 0; i <= nLevels; i++) {
				levels[i].colors[0] = static_cast<uint8_t>(levels[i].colors[0]*255);
				levels[i].colors[1] = static_cast<uint8_t>(levels[i].colors[1]*255);
				levels[i].colors[2] = static_cast<uint8_t>(levels[i].colors[2]*255);
			}
			GXint32 generatedHeightTexSize = NoiseGeneratorLayer::texHeight * NoiseGeneratorLayer::texWidth * 3;
			GXint32 texSize = width * depth * 3;
			GXFloat* heights = new GXFloat[width * depth];
			texColors = new GXuint8[texSize];
			GXint32 widthFactor = width / NoiseGeneratorLayer::texWidth;
			GXint32 depthFactor = depth / NoiseGeneratorLayer::texHeight;
			GXint32 stepX = widthFactor == 0 ? 1 : widthFactor;
			GXint32 stepZ = depthFactor == 0 ? 1 : depthFactor;
			GXint32 iteWidth = 0;
			GXint32 iteDepth = 0;
		
			GXint32 currX = 0;
			GXint32 currZ = 0;
			GXint32 k = 0;
			GXint32 l = 0;
			for (GXint32 z = 0; z < depth; z++) {
				iteWidth = 0;
				for (GXint32 x = 0; x < width; x++) {
					GXint32 loc = iteDepth * NoiseGeneratorLayer::texWidth + iteWidth;
					heights[k] = NoiseGeneratorLayer::heightsNormalized[loc]*height;
					//set base color first
					texColors[l] = static_cast<GXuint8>(levels[nLevels].colors[0]);
					texColors[l + 1] = static_cast<GXuint8>(levels[nLevels].colors[1]);
					texColors[l + 2] = static_cast<GXuint8>(levels[nLevels].colors[2]);
					for (GXint32 i = 0; i < nLevels; i++) {
						if (NoiseGeneratorLayer::heightsNormalized[loc] > levels[i].heightVal) {
							texColors[l] = static_cast<GXuint8>(levels[i].colors[0]);
							texColors[l + 1] = static_cast<GXuint8>(levels[i].colors[1]);
							texColors[l + 2] = static_cast<GXuint8>(levels[i].colors[2]);
							break;
						}
					}
					currX++;
					if (currX == stepX) {
						currX = 0;
						iteWidth++;
					}
					l+=3;
					k++;
				}
				currZ++;
				if (currZ == stepZ) {
					currZ = 0;
					iteDepth++;
				}
			}
			plane.reset(new GXPlane(width, depth,clusteringFactorX,clusteringFactorZ));
			plane->init(heights);
			
			isUpdated = true;
			delete[] heights;

			
		}

	};

}
