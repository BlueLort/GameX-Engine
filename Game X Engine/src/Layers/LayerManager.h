#pragma once
#include "Layers/Main/MainScene.h"
#include "Layers/Debug/Logging/LogLayer.h"
#include "Layers/Utility/NoiseGeneratorLayer.h"

namespace gx {
	constexpr int32_t SCENE_WIDTH = 1920;
	constexpr int32_t SCENE_HEIGHT = 1080;
	class GX_DLL LayerManager {
	public:
		inline static LayerManager& getInstance(){
			static LayerManager LM=LayerManager();
			return LM;
		}
		inline void init() {
			mainSceneLayer = std::make_shared<MainScene>("Scene", SCENE_WIDTH, SCENE_HEIGHT);
			mainSceneLayer->init();
			logLayer = std::make_shared<LogLayer>("Log");
			logLayer->init();
			ngLayer = std::make_shared<NoiseGeneratorLayer>("Height Map Settings");
			layers.add(std::make_pair(1/*high Priority -> Handled first*/, mainSceneLayer));
			layers.add(std::make_pair(0, logLayer));
			layers.add(std::make_pair(0, ngLayer));
			ngLayer->generateNoiseMap();//generate some noisemap initially.
			//ngLayer->onUpdate(0.0f);//update the gui and create texture for some random height map
		}
		inline void addModelObject(std::shared_ptr<GXModelObject>& obj) {
			if (obj->isReady)
				mainSceneLayer->addModelObject(obj);
			else {
				modelObjectRequests.emplace(obj);
			}
				
		}
		inline void setPlane(std::shared_ptr<GXPlane>& plane) {
			if (plane->isReady)
				mainSceneLayer->setPlane(plane);
			else {
				//modelObjectRequests.emplace(obj);
			}

		}
		inline void addLog(const char* txt) {
			logLayer->addLog(txt);
		}
		inline bool isMainSceneSelected() {
			return mainSceneLayer->isSelected();
		}
		inline void renderUpdateLayers(float deltaTime) {
			if (!modelObjectRequests.empty()) {
				if (modelObjectRequests.front()->isReady) {
					mainSceneLayer->addModelObject(modelObjectRequests.front());
					modelObjectRequests.pop();
				}
			}
			layers.renderUpdateLayers(deltaTime);
		}
		inline void onGUIRender() {
			layers.onGUIRender();
		}
		inline const float* getHeightsNormalized()const { return NoiseGeneratorLayer::getHeightsNormalized(); }
	private:
		inline LayerManager() { 
			
		}
		std::queue<std::shared_ptr<GXModelObject>> modelObjectRequests;
		std::shared_ptr<MainScene> mainSceneLayer;
		std::shared_ptr<LogLayer> logLayer;
		std::shared_ptr<NoiseGeneratorLayer> ngLayer;
		LayerQueue layers;
	};

}