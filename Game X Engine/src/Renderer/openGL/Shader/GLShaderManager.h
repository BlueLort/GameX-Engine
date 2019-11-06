#pragma once

#include "pch.h"
#include "Config.h"
#include "GLShader.h"
namespace gx {
	//0 vert 1 frag 2 geo
	extern const char* GLDefaultShader[3];
	enum GLShaderType
	{
		DEFAULT = 0//ADD MORE TYPES THAT ARE PRECOMPILED FOR THE USER
	};
	class GLShader;
	class GX_DLL GLShaderManager {
	public:
		static void init();

		inline static GLShader* getShader(GLShaderType type) { return defaultShaders[type]; };

		static void destroy();
	private:
		static std::unordered_map<GLShaderType,GLShader*> defaultShaders;
		



	};

}