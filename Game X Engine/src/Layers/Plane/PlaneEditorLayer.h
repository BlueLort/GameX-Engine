#pragma once
#include "Config.h"
#include "Layers/Layer.h"
#include "Layers/Utility/NoiseGeneratorLayer.h"
#include "Terrain/Plane/Plane.h"
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
		inline LevelData() :heightVal(0.0f) {
			colors[0] = 0.0f;
			colors[1] = 0.0f;
			colors[2] = 0.0f;
		}
	};
	class GX_DLL PlaneEditorLayer :public Layer {
	public:
		inline PlaneEditorLayer(const std::string& layerName) : Layer(layerName),width(256),depth(256), texID(0) {
			scale[0] = 1;//x
			scale[1] = 1;//y
			scale[2] = 1;//z
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
		uint8_t* texColors;
		static bool isUpdated;
		static bool canUpdate;
		int width;
		int depth;
		int height;
		float scale[3];
		std::vector<LevelData> levels;
		std::future<void> asyncTask;


	};

}
