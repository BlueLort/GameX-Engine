#include "pch.h"
#include "GLShaderManager.h"


namespace gx {

	//REFERENCES DEFAULT SHADERS USING THE SHADER TYPE
	std::vector<GXShader*> GLShaderManager::defaultShaders;
	std::unordered_map<const char*, GXShader*>  GLShaderManager::customShaders;

	void GLShaderManager::init()
	{
		//DEFAULT SHADERS COMPILATION

		GXShader* sh = new GXShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultColorShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultColorShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultColorShader[2]);
		defaultShaders.push_back(sh);
		GXE_DEBUG("Default Color GLShader Compiled Successfully");

		sh = new GXShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultLightingShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultLightingShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultLightingShader[2]);
		defaultShaders.push_back(sh);
		GXE_DEBUG("Default Lighting GLShader Compiled Successfully");
		sh = new GXShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultModelShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultModelShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultModelShader[2]);
		defaultShaders.push_back(sh);
		GXE_DEBUG("Default Model GLShader Compiled Successfully");

		sh = new GXShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultSkydomeShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultSkydomeShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultSkydomeShader[2]);
		defaultShaders.push_back(sh);
		GXE_DEBUG("Default Skydome GLShader Compiled Successfully");

		sh = new GXShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultPlaneShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultPlaneShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultPlaneShader[2]);
		defaultShaders.push_back(sh);
		GXE_DEBUG("Default Plane GLShader Compiled Successfully");

		sh = new GXShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultGBufferShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultGBufferShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultGBufferShader[2]);
		defaultShaders.push_back(sh);
		GXE_DEBUG("Default GBuffer GLShader Compiled Successfully");

		

		sh = new GXShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultDeferredShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultDeferredShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultDeferredShader[2]);
		defaultShaders.push_back(sh);
		GXE_DEBUG("Default Deferred Lighting Pass Shader Compiled Successfully");

		sh = new GXShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultOutlineShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultOutlineShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultOutlineShader[2]);
		defaultShaders.push_back(sh);
		GXE_DEBUG("Default Outline Shader Compiled Successfully");

		sh = new GXShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultGridShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultGridShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultGridShader[2]);
		defaultShaders.push_back(sh);
		GXE_DEBUG("Default Grid Shader Compiled Successfully");

		sh = new GXShader();
		sh->addShader(GX_VERTEX_SHADER, GLDefaultShadowMapShader[0]);
		sh->addShader(GX_FRAGMENT_SHADER, GLDefaultShadowMapShader[1]);
		sh->addShader(GX_GEOMETRY_SHADER, GLDefaultShadowMapShader[2]);
		defaultShaders.push_back(sh);
		GXE_DEBUG("Default Shadow Map Shader Compiled Successfully");
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
	GXShader* GLShaderManager::createShader(const char* filePath)
	{
		/*
		const char* cusotmShader[3];
		//GET THE CUSTOM SHADER .. 

		//TODO READ IT FROM FILE AND PARSE IT USING IOManager

		customShaders[filePath] = new GLShader(cusotmShader[0], cusotmShader[1], cusotmShader[2]);
		GXE_DEBUG("Custom GLShader Compiled Successfully,\nPath: {0}", filePath);
		return customShaders[filePath];
		*/
		return new GXShader();
	}

}