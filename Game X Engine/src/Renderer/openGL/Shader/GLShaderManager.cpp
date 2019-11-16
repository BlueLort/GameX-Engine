#include "pch.h"
#include "GLShaderManager.h"


namespace gx {

	//REFERENCES DEFAULT SHADERS USING THE SHADER TYPE
	std::vector<GLShader*> GLShaderManager::defaultShaders;
	std::unordered_map<const char*, GLShader*>  GLShaderManager::customShaders;

	void GLShaderManager::init()
	{
		//DEFAULT SHADER COMPILATION
		defaultShaders.emplace_back(new GLShader(GLDefaultShader[0], GLDefaultShader[1], GLDefaultShader[2]));
		GXE_DEBUG("Default GLShader Compiled Successfully");
		


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

	GLShader* GLShaderManager::createShader(const char* filePath)
	{
		const char* cusotmShader[3];
		//GET THE CUSTOM SHADER .. 

		//TODO READ IT FROM FILE AND PARSE IT USING IOManager

		customShaders[filePath] = new GLShader(cusotmShader[0], cusotmShader[1], cusotmShader[2]);
		GXE_DEBUG("Custom GLShader Compiled Successfully,\nPath: {0}",filePath);
		return customShaders[filePath];
	}

}