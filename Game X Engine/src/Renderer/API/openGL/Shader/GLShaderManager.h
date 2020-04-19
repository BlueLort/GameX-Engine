#pragma once

#include "pch.h"
#include "Config.h"
#include "Renderer/Enums.h"
#include "Renderer/Shader/GXShader.h"

namespace gx {
	//0 vert 1 frag 2 geo
	extern const char* GLDefaultColorShader[3];
	extern const char* GLDefaultModelShader[3];
	extern const char* GLDefaultLightingShader[3];
	extern const char* GLDefaultSkydomeShader[3];
	extern const char* GLDefaultPlaneShader[3];
	extern const char* GLDefaultGBufferShader[3];
	extern const char* GLDefaultDeferredShader[3];
	extern const char* GLDefaultOutlineShader[3];
	class GX_DLL GLShaderManager {
	public:
		GLShaderManager() = delete;
		static void init();

		static GXShader* getShader(GXCompiledShader type) { return defaultShaders[type]; };
		static GXShader* getShader(const char* filePath) { 
			auto ite = customShaders.find(filePath);
			if (ite != customShaders.end()) {
				return ite->second;
			}
			return createShader(filePath);
		};
		
		static void destroy();
	private:
		static std::vector<GXShader*> defaultShaders;
		static std::unordered_map<const char*, GXShader*> customShaders;

		static GXShader* createShader(const char* filePath);


	};

}