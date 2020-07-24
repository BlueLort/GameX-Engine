#pragma once
#include "Config.h"
#include "Layers/Layer.h"
#include "Terrain/HeightGenerators/NoiseGenerator.h"
#include "Terrain/HeightGenerators/TextureMapGenerator.h"

constexpr GXint32 PERLIN_WIDTH = 512;
constexpr GXint32 PERLIN_HEIGHT = 512;
namespace gx {
	class GX_DLL NoiseGeneratorLayer :public Layer {
	public:
		inline NoiseGeneratorLayer(const std::string& layerName) : Layer(layerName),
			scale(27.0f), nOctaves(4), persistence(0.5f), lacunarity(2.9f), z(24.0f){
			filePathBuffer[0] = '\0';
			fileDialog.SetTitle("File Browser");
			fileDialog.SetTypeFilters({ ".jpg", ".png" });
		}
		virtual void init()override;
		virtual void destroy()override;
		virtual void start()override;
		virtual void end()override;
		virtual GXint32 onEvent(const gx::event::GXEventType& eventType)override;
		virtual void onUpdate(float deltaTime)override;
		virtual void onGUIRender()override;
		static const float* getHeightsNormalized(){ return heightsNormalized; }
		static inline void generateNoiseMap(GXint32 width=PERLIN_WIDTH, GXint32 height = PERLIN_HEIGHT, float scale=27.0f, GXint32 nOctaves=4, float persistence=0.33f, float lacunarity=2.9f, float z=0.0f) {
			NoiseGenerator ng(width, height, scale, nOctaves, persistence, lacunarity, z);
			ng.init();
			heightValues = ng.getHeightsColor();
			heightsNormalized = ng.getHeightsNormalized();
			isUpdated = true;

		}
		static inline void createHeightMapAsync(const char* filePath, GXTexture2DType type) {

			TextureMapGenerator tmg(filePath);
			tmg.init();
			heightValues = tmg.getHeightsColor();
			if (heightValues == nullptr) {
				errorLoading = true;
				canUpdate = true;
				return;
			}
			texWidth = tmg.getWidth();
			texHeight = tmg.getHeight();
			isUpdated = true;

		}
		static GXuint32 texID;
	private:
		ImGui::FileBrowser fileDialog;
		ImGuiWindowFlags windowFlags;
		static uint8_t* heightValues;
		static float* heightsNormalized;
		static bool isUpdated;
		static bool canUpdate;
		static bool errorLoading;
		static GXint32 texWidth;
		static GXint32 texHeight;
		GXint32 nOctaves;
		float scale, persistence,lacunarity,z;
		char filePathBuffer[1024];
		inline void resetHeightValues() {
			errorLoading = false;
			texWidth = PERLIN_WIDTH;
			texHeight = PERLIN_HEIGHT;
			if (heightValues != nullptr) {
				delete[] heightValues;
				heightValues = nullptr;
			}
			if (heightsNormalized != nullptr) {
				delete[] heightsNormalized;
				heightsNormalized = nullptr;
			}
		}
	
		std::future<void> asyncTask;
		inline void createPerlinTex() {
			canUpdate = false;
			resetHeightValues();
			asyncTask=std::async(std::launch::async, generateNoiseMap, texWidth, texHeight, scale, nOctaves, persistence, lacunarity, z);
		}
		inline void createHMapTex(const char* filePath) {
			canUpdate = false;
			resetHeightValues();
			asyncTask = std::async(std::launch::async, createHeightMapAsync, filePath,GX_HEIGHT);
		
		}
		
		friend class PlaneEditorLayer;
	};

}
