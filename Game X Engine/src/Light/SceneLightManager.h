#pragma once
#include "Config.h"
#include "pch.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
namespace gx {
	class GX_DLL SceneLightManager {
	public:
		static SceneLightManager& getInstance(){
			static SceneLightManager slm;
			return slm;
		}
		void init() {
			
			std::shared_ptr<PointLight> pLight;
			pLight.reset(new PointLight(GXVec3(0.0f,3.0f,3.0f)));
			pLights.emplace_back(pLight);
			flashLight.reset(new SpotLight(GXVec3(0.0f,5.0f,1.0f),GXVec3(0.0f,0.0f,-1.0f)));
			dirLight.reset(new DirectionalLight(GXVec3(-0.2f, -1.0f, -0.3f)));
		}
		std::vector<std::shared_ptr<PointLight>> getPointLights() const { return pLights; }
		std::shared_ptr<SpotLight> getSpotLight() const { return flashLight; }
		std::shared_ptr<DirectionalLight> getDirLight() const { return dirLight; }
	private:
		SceneLightManager() {

		}
		std::vector<std::shared_ptr<PointLight>> pLights;
		std::shared_ptr<SpotLight> flashLight;
		std::shared_ptr<DirectionalLight> dirLight;

	};
}