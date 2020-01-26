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
			dirLight.reset(new DirectionalLight(GXVec3(-0.2f, -1.0f, -3.3f)));
		}
		std::vector<std::shared_ptr<PointLight>> getPointLights() const { return pLights; }
		std::shared_ptr<SpotLight> getSpotLight() const { return flashLight; }
		std::shared_ptr<DirectionalLight> getDirLight() const { return dirLight; }
		inline void setLightValues(GLShader* glshader) {
			//Directional light
			glshader->setVec3("dirLight.direction", dirLight->direction);
			glshader->setVec3("dirLight.ambient", dirLight->ambientColor);
			glshader->setVec3("dirLight.diffuse", dirLight->diffuseColor);
			glshader->setVec3("dirLight.specular", dirLight->specularColor);
			for (int i = 0; i < pLights.size();i++) {
				//TODO IMPROVE THIS
				std::string loc = "pointLights[" + std::to_string(i)+"]";
				glshader->setVec3(std::string(loc+".position").c_str(), pLights[i]->position);
				glshader->setVec3(std::string(loc + ".ambient").c_str(), pLights[i]->ambientColor);
				glshader->setVec3(std::string(loc + ".diffuse").c_str(), pLights[i]->diffuseColor);
				glshader->setVec3(std::string(loc + ".specular").c_str(), pLights[i]->specularColor);
				glshader->setFloat(std::string(loc + ".constant").c_str(), pLights[i]->constant);
				glshader->setFloat(std::string(loc + ".linear").c_str(), pLights[i]->linear);
				glshader->setFloat(std::string(loc + ".quadratic").c_str(), pLights[i]->quadratic);
			}
			// spotLight
			glshader->setVec3("spotLight.position", flashLight->position);
			glshader->setVec3("spotLight.direction", flashLight->direction);
			glshader->setVec3("spotLight.ambient", flashLight->ambientColor);
			glshader->setVec3("spotLight.diffuse", flashLight->diffuseColor);
			glshader->setVec3("spotLight.specular", flashLight->specularColor);
			glshader->setFloat("spotLight.constant", flashLight->constant);
			glshader->setFloat("spotLight.linear", flashLight->linear);
			glshader->setFloat("spotLight.quadratic", flashLight->quadratic);
			glshader->setFloat("spotLight.cutOff", flashLight->cutOff);
			glshader->setFloat("spotLight.outerCutOff", flashLight->outerCutOff);
		}
	private:
		SceneLightManager() {

		}
		std::vector<std::shared_ptr<PointLight>> pLights;
		std::shared_ptr<SpotLight> flashLight;
		std::shared_ptr<DirectionalLight> dirLight;

	};
}