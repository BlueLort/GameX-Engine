#include "pch.h"
#include "GLShaderManager.h"


namespace gx {

	//REFERENCES DEFAULT SHADERS USING THE SHADER TYPE
	std::vector<GLShader*> GLShaderManager::defaultShaders;
	std::unordered_map<const char*, GLShader*>  GLShaderManager::customShaders;

	void GLShaderManager::init()
	{
		//DEFAULT SHADERS COMPILATION

		GLShader* sh = new GLShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultColorShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultColorShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultColorShader[2]);
		defaultShaders.emplace_back(sh);
		GXE_DEBUG("Default Color GLShader Compiled Successfully");

		sh = new GLShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultLightingShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultLightingShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultLightingShader[2]);
		defaultShaders.emplace_back(sh);
		GXE_DEBUG("Default Lighting GLShader Compiled Successfully");
		sh = new GLShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultModelShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultModelShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultModelShader[2]);
		defaultShaders.emplace_back(sh);
		GXE_DEBUG("Default Model GLShader Compiled Successfully");

		sh = new GLShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultSkydomeShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultSkydomeShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultSkydomeShader[2]);
		defaultShaders.emplace_back(sh);
		GXE_DEBUG("Default Skydome GLShader Compiled Successfully");



	}

	void GLShaderManager::destroy()
	{
		for (auto ite : defaultShaders) {
			delete ite;
		}
		for (auto ite : customShaders) {
			delete ite.second;
		}
	}

	//TODO EDIT THIS TO MAKE IT ADAPT WITH NEW SHADER LINKAGE METHOD
	GLShader* GLShaderManager::createShader(const char* filePath)
	{
		/*
		const char* cusotmShader[3];
		//GET THE CUSTOM SHADER .. 

		//TODO READ IT FROM FILE AND PARSE IT USING IOManager

		customShaders[filePath] = new GLShader(cusotmShader[0], cusotmShader[1], cusotmShader[2]);
		GXE_DEBUG("Custom GLShader Compiled Successfully,\nPath: {0}", filePath);
		return customShaders[filePath];
		*/
		return new GLShader();
	}

}