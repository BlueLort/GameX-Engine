#pragma once
#include "Config.h"
#include "Layers/Layer.h"
#include "Terrain/HeightGenerators/NoiseGenerator.h"

namespace gx {
	class GX_DLL NoiseGeneratorLayer :public Layer {
	public:
		inline NoiseGeneratorLayer(const std::string& layerName) : Layer(layerName),
			scale(27.0f), nOctaves(4), persistence(0.5f), lacunarity(2.9f), z(24.0f), currentState(0) ,texID(0)
			,texWidth(256),texHeight(256){
			filePathBuffer[0] = '\0';
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
		static uint8_t* heightValues;
		static float* heightsNormalized;
		static bool isUpdated;
		static bool canUpdate;
		int currentState;//0 is perlin , 1 is heightmap image
		int texWidth, texHeight, nOctaves;
		float scale, persistence,lacunarity,z;
		char filePathBuffer[1024];
		
		static inline void generateNoiseMap(int width, int height, float scale,int nOctaves, float persistence, float lacunarity, float z) {
			NoiseGenerator ng(width, height, scale, nOctaves, persistence, lacunarity, z);
			ng.init();
			heightValues = ng.getHeightsColor();
			heightsNormalized = ng.getHeightsNormalized();
			isUpdated = true;
			
		}
		std::future<void> asyncTask;
		std::thread generationThread;
		inline void createPerlinTex() {
			canUpdate = false;
			//t=std::thread(generateNoiseMap, texWidth, texHeight, scale, nOctaves, persistence, lacunarity, z);
			asyncTask=std::async(std::launch::async, generateNoiseMap, texWidth, texHeight, scale, nOctaves, persistence, lacunarity, z);
			//generateNoiseMap(texWidth, texHeight, scale, nOctaves, persistence, lacunarity, z);
		}
		inline void createHMapTex() {
			//std::async(std::launch::async, generateNoiseMap, texWidth, texHeight, scale, nOctaves, persistence, lacunarity, z);
		}
		

	};

}
