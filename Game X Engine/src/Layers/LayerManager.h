#pragma once
#include "Layers/Main/MainScene.h"
#include "Layers/Debug/Logging/LogLayer.h"
#include "Layers/Utility/NoiseGeneratorLayer.h"

namespace gx {
	constexpr int32_t SCENE_WIDTH = 1920;
	constexpr int32_t SCENE_HEIGHT = 1080;
	class GX_DLL LayerManager {
	public:
		inline static LayerManager getInstance(){
			static LayerManager LM=LayerManager();
			return LM;
		}
		inline void addModelObject(std::shared_ptr<GXModelObject>& obj) {
			mainSceneLayer->addModelObject(obj);
		}
		inline void addLog(const char* txt) {
			logLayer->addLog(txt);
		}
		inline bool isMainSceneSelected() {
			return mainSceneLayer->isSelected();
		}
		inline void renderUpdateLayers(float deltaTime) {
			layers.renderUpdateLayers(deltaTime);
		}
		inline void onGUIRender() {
			layers.onGUIRender();
		}
	private:
		inline LayerManager() {
			mainSceneLayer = std::make_shared<MainScene>("Scene", SCENE_WIDTH, SCENE_HEIGHT);
			mainSceneLayer->init();
			logLayer = std::make_shared<LogLayer>("Log");
			logLayer->init();
			ngLayer = std::make_shared<NoiseGeneratorLayer>("Height Map Settings");
			layers.add(std::make_pair(1/*high Priority -> Handled first*/, mainSceneLayer));
			layers.add(std::make_pair(0, logLayer));
			layers.add(std::make_pair(0, ngLayer));
			
		}
		
		std::shared_ptr<MainScene> mainSceneLayer;
		std::shared_ptr<LogLayer> logLayer;
		std::shared_ptr<NoiseGeneratorLayer> ngLayer;
		LayerQueue layers;
	};

}