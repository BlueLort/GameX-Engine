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
			, width(2), depth(2),height(6), texID(0), rowInstances(16), colInstances(16),xDensity(16),zDensity(16){
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
		GXint32 rowInstances;
		GXint32 colInstances;
		GXint32 xDensity;
		GXint32 zDensity;
		std::vector<LevelData> levels;
		std::future<void> asyncTask;

		static inline void createColoredTexture(GXint32 width, GXint32 depth, GXint32 height
			,GXint32 rowInstances,GXint32 colInstances,
			GXint32 clusteringFactorX, GXint32 clusteringFactorZ, std::vector<LevelData> levels) {
			if (NoiseGeneratorLayer::heightValues == nullptr) {
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
				levels[i].colors[0] = static_cast<uint8_t>(levels[i].colors[0]*255.0f);
				levels[i].colors[1] = static_cast<uint8_t>(levels[i].colors[1]*255.0f);
				levels[i].colors[2] = static_cast<uint8_t>(levels[i].colors[2]*255.0f);
			}
			GXint32 generatedHeightTexSize = NoiseGeneratorLayer::texHeight * NoiseGeneratorLayer::texWidth * 3;
			texColors = new GXuint8[generatedHeightTexSize];
			for (GXint32 z = 0; z < NoiseGeneratorLayer::texHeight; z++) {
				for (GXint32 x = 0; x < NoiseGeneratorLayer::texWidth; x++) {
					GXint32 loc = (z * NoiseGeneratorLayer::texWidth + x) * 3;
					//set base color first
					texColors[loc] = static_cast<GXuint8>(levels[nLevels].colors[0]);
					texColors[loc + 1] = static_cast<GXuint8>(levels[nLevels].colors[1]);
					texColors[loc + 2] = static_cast<GXuint8>(levels[nLevels].colors[2]);
					GXFloat currHeight = NoiseGeneratorLayer::heightValues[loc] / 255.0f;
					
					for (GXint32 i = 0; i < nLevels; i++) {
						if (currHeight > levels[i].heightVal) {
							texColors[loc] = static_cast<GXuint8>(levels[i].colors[0]);
							texColors[loc + 1] = static_cast<GXuint8>(levels[i].colors[1]);
							texColors[loc + 2] = static_cast<GXuint8>(levels[i].colors[2]);
							break;
						}
					}
				}
			}
			plane.reset(new GXPlane(width, depth,height,rowInstances,colInstances,clusteringFactorX,clusteringFactorZ));
			plane->init();
			
			isUpdated = true;

			
		}

	};

}
