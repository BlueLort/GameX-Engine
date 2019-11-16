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
		GLShaderManager() = delete;
		static void init();

		inline static GLShader* getShader(GLShaderType type) { return defaultShaders[type]; };
		inline static GLShader* getShader(const char* filePath) { 
			auto ite = customShaders.find(filePath);
			if (ite != customShaders.end()) {
				return ite->second;
			}
			return createShader(filePath);
		};
		
		static void destroy();
	private:
		static std::vector<GLShader*> defaultShaders;
		static std::unordered_map<const char*, GLShader*> customShaders;

		static GLShader* createShader(const char* filePath);


	};

}