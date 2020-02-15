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
		float colors[3];
		float heightVal;
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
		virtual int onEvent(const gx::event::GXEventType& eventType)override;
		virtual void onUpdate(float deltaTime)override;
		virtual void onGUIRender()override;

	private:
		ImGuiWindowFlags windowFlags;
		uint32_t texID;
		static uint8_t* texColors;
		static std::shared_ptr<GXPlane> plane;
		static bool isUpdated;
		static bool canUpdate;
		int width;
		int depth;
		int height;
		int clusteringFactorX;
		int clusteringFactorZ;
		std::vector<LevelData> levels;
		std::future<void> asyncTask;

		static inline void createColoredTexture(int width, int depth, int height, int clusteringFactorX, int clusteringFactorZ, std::vector<LevelData> levels) {
			
			if (NoiseGeneratorLayer::heightsNormalized == nullptr|| !NoiseGeneratorLayer::canUpdate ) {
				canUpdate = true;
				return;
			}
			//levels sent by copy so i can do whatever i want on them
			std::sort(levels.begin(), levels.end(), [](LevelData& l1, LevelData& l2) {
				return l1.heightVal > l2.heightVal;
				});
			int nLevels = levels.size();
			nLevels--;
			for (int i = 0; i <= nLevels; i++) {
				levels[i].colors[0] = static_cast<uint8_t>(levels[i].colors[0]*255);
				levels[i].colors[1] = static_cast<uint8_t>(levels[i].colors[1]*255);
				levels[i].colors[2] = static_cast<uint8_t>(levels[i].colors[2]*255);
			}
			int generatedHeightTexSize = NoiseGeneratorLayer::texHeight * NoiseGeneratorLayer::texWidth * 3;
			int texSize = width * depth * 3;
			float* heights = new float[width * depth];
			texColors = new uint8_t[texSize];
			int widthFactor = width / NoiseGeneratorLayer::texWidth;
			int depthFactor = depth / NoiseGeneratorLayer::texHeight;
			int stepX = widthFactor == 0 ? 1 : widthFactor;
			int stepZ = depthFactor == 0 ? 1 : depthFactor;
			int iteWidth = 0;
			int iteDepth = 0;
		
			int currX = 0;
			int currZ = 0;
			int k = 0;
			int l = 0;
			for (int z = 0; z < depth; z++) {
				iteWidth = 0;
				for (int x = 0; x < width; x++) {
					int loc = iteDepth * NoiseGeneratorLayer::texWidth + iteWidth;
					heights[k] = NoiseGeneratorLayer::heightsNormalized[loc]*height;
					//set base color first
					texColors[l] = levels[nLevels].colors[0];
					texColors[l + 1] = levels[nLevels].colors[1];
					texColors[l + 2] = levels[nLevels].colors[2];
					for (int i = 0; i < nLevels; i++) {
						if (NoiseGeneratorLayer::heightsNormalized[loc] > levels[i].heightVal) {
							texColors[l] = levels[i].colors[0];
							texColors[l + 1] = levels[i].colors[1];
							texColors[l + 2] = levels[i].colors[2];
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
