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
			dirLight.reset(new DirectionalLight(GXVec3(-0.2f, 1.0f, -3.3f)));
		}
		std::vector<std::shared_ptr<PointLight>> getPointLights() const { return pLights; }
		std::shared_ptr<SpotLight> getSpotLight() const { return flashLight; }
		std::shared_ptr<DirectionalLight> getDirLight() const { return dirLight; }
		inline void setLightValues(GXShader* shader) {
			//Directional light
			shader->setMat4("lightSpaceMatrix", dirLight->lightSpaceMatrix);
			shader->setVec3("lightPos", dirLight->virtualPos);
			shader->setVec3("dirLight.direction", dirLight->direction);
			shader->setVec3("dirLight.ambient", dirLight->ambientColor);
			shader->setVec3("dirLight.diffuse", dirLight->diffuseColor);
			shader->setVec3("dirLight.specular", dirLight->specularColor);
			for (GXint32 i = 0; i < pLights.size();i++) {
				//TODO IMPROVE THIS
				std::string loc = "pointLights[" + std::to_string(i)+"]";
				shader->setVec3(std::string(loc+".position").c_str(), pLights[i]->position);
				shader->setVec3(std::string(loc + ".ambient").c_str(), pLights[i]->ambientColor);
				shader->setVec3(std::string(loc + ".diffuse").c_str(), pLights[i]->diffuseColor);
				shader->setVec3(std::string(loc + ".specular").c_str(), pLights[i]->specularColor);
				shader->setFloat(std::string(loc + ".constant").c_str(), pLights[i]->constant);
				shader->setFloat(std::string(loc + ".linear").c_str(), pLights[i]->linear);
				shader->setFloat(std::string(loc + ".quadratic").c_str(), pLights[i]->quadratic);
			}
			// spotLight
			shader->setVec3("spotLight.position", flashLight->position);
			shader->setVec3("spotLight.direction", flashLight->direction);
			shader->setVec3("spotLight.ambient", flashLight->ambientColor);
			shader->setVec3("spotLight.diffuse", flashLight->diffuseColor);
			shader->setVec3("spotLight.specular", flashLight->specularColor);
			shader->setFloat("spotLight.constant", flashLight->constant);
			shader->setFloat("spotLight.linear", flashLight->linear);
			shader->setFloat("spotLight.quadratic", flashLight->quadratic);
			shader->setFloat("spotLight.cutOff", flashLight->cutOff);
			shader->setFloat("spotLight.outerCutOff", flashLight->outerCutOff);
		}
	private:
		SceneLightManager() {

		}
		std::vector<std::shared_ptr<PointLight>> pLights;
		std::shared_ptr<SpotLight> flashLight;
		std::shared_ptr<DirectionalLight> dirLight;

	};
}