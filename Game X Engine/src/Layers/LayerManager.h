#pragma once
#include "Layers/Main/MainScene.h"
#include "Layers/Debug/Logging/LogLayer.h"
#include "Layers/Utility/NoiseGeneratorLayer.h"
#include "Layers/Plane/PlaneEditorLayer.h"
namespace gx {
	
	constexpr GXint32 SCENE_WIDTH = 1920;
	constexpr GXint32 SCENE_HEIGHT = 1080;
	class GX_DLL LayerManager {
	public:
		inline static LayerManager& getInstance(){
			static LayerManager LM=LayerManager();
			return LM;
		}
		inline void init() {
			mainSceneLayer = std::make_shared<MainScene>("Scene", SCENE_WIDTH, SCENE_HEIGHT);
			mainSceneLayer->init();
			logLayer = std::make_shared<LogLayer>("GX Console");
			logLayer->init();
			noiseGenerationLayer = std::make_shared<NoiseGeneratorLayer>("Height Map Settings");
			planeEditorLayer= std::make_shared<PlaneEditorLayer>("Terrain Settings");
			layers.add(std::make_pair(0/*low number -> Drawn first*/, mainSceneLayer));
			layers.add(std::make_pair(1, logLayer));
			layers.add(std::make_pair(1, noiseGenerationLayer));
			layers.add(std::make_pair(1, planeEditorLayer));
		}
		inline void addModelObject(std::shared_ptr<GXModelObject>& obj) {
			if (obj->isReady)
				mainSceneLayer->addModelObject(obj);
			else {
				modelObjectRequests.emplace(obj);
			}
				
		}
		inline std::shared_ptr<GXModelObject> getModelObject(GXuint32 GXID) {
			return mainSceneLayer->getModelObject(GXID);
		}
		inline void GXPrintf(const char* fmt,...) {
			va_list args;
			va_start(args, fmt);
			logLayer->addLog(fmt, args);
			va_end(args);
		}
		inline bool isMainSceneSelected() {
			return mainSceneLayer->isSelected();
		}
		inline std::pair<float, float> getSceneMouseLocNormalized() {
			return mainSceneLayer->getMouseLocNormalized();
		}
		inline std::pair<GXint32,GXint32> getSceneMouseLoc() {
			return mainSceneLayer->getMouseLoc();
		}
		inline void mousePressRequest() {
			mainSceneLayer->mousePressRequest();
		}
		inline void renderUpdateLayers(float deltaTime) {
			if (!modelObjectRequests.empty()) {
				if (modelObjectRequests.front()->isReady) {
					modelObjectRequests.front()->fixComponentsID();
					mainSceneLayer->addModelObject(modelObjectRequests.front());
					modelObjectRequests.pop();
				}
			}
			layers.renderUpdateLayers(deltaTime);

		}
		inline void onGUIRender() {
			layers.onGUIRender();
		}
		
	private:
		inline LayerManager() { 
			
		}
		std::queue<std::shared_ptr<GXModelObject>> modelObjectRequests;
		std::shared_ptr<MainScene> mainSceneLayer;
		std::shared_ptr<LogLayer> logLayer;
		std::shared_ptr<NoiseGeneratorLayer> noiseGenerationLayer;
		std::shared_ptr<PlaneEditorLayer> planeEditorLayer;
		LayerQueue layers;
	};

}