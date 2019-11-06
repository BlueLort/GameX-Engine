#include "pch.h"
#include "GLShaderManager.h"


namespace gx {

	std::unordered_map<GLShaderType, GLShader*> GLShaderManager::defaultShaders;

	void GLShaderManager::init()
	{
		//DEFAULT SHADER COMPILATION
		GLShader* defaultShader= new GLShader(GLDefaultShader[0], GLDefaultShader[1], GLDefaultShader[2]);
		defaultShaders[GLShaderType::DEFAULT] = defaultShader;
		GXE_DEBUG("Default GLShader Compiled Successfully");
		


	}

	void GLShaderManager::destroy()
	{
		for (auto ite : defaultShaders) {
			delete ite.second;
		}
	}

}