#pragma once
#include "Layers/Main/MainScene.h"
#include "Layers/Debug/Logging/LogLayer.h"
namespace gx {
	constexpr int32_t SCENE_WIDTH = 1920;
	constexpr int32_t SCENE_HEIGHT = 1080;
	class GX_DLL LayerManager {
	public:
		static void init() {
			mainSceneLayer= std::make_shared<MainScene>("Scene");
			mainSceneLayer->init(SCENE_WIDTH, SCENE_HEIGHT);
			logLayer = std::make_shared<LogLayer>("Log");
			logLayer->init(400, 300);
			layers.add(std::make_pair(1/*high Priority -> Drawn first*/, mainSceneLayer));
			layers.add(std::make_pair(0, logLayer));
		}
		inline static void addObject(std::shared_ptr<GXModelObject>& obj) {
			mainSceneLayer->addModelObject(obj);
		}
		inline static void addLog(const char* txt) {
			logLayer->addLog(txt);
		}
		inline static bool isMainSceneSelected() {
			return mainSceneLayer->isSelected();
		}
		inline static void renderUpdateLayers(float deltaTime) {
			layers.renderUpdateLayers(deltaTime);
		}
		inline static void onGUIRender() {
			layers.onGUIRender();
		}
	private:
		static std::shared_ptr<MainScene> mainSceneLayer;
		static std::shared_ptr<LogLayer> logLayer;
		static LayerQueue layers;
		


	};
}