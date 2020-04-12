#pragma once

#include "pch.h"
#include "Config.h"
#include "Renderer/Shader/GXShader.h"
#ifdef USING_OPENGL
	#include "Renderer/API/openGL/Shader/GLShaderManager.h"
#endif
namespace gx {
#ifdef USING_OPENGL
	class GX_DLL GXShaderManager {
	public:
		GXShaderManager() = delete;
		inline static void init() {
			GLShaderManager::init();
		}

		inline static GXShader* getShader(GXCompiledShader type) { return GLShaderManager::getShader(type); };
		inline static GXShader* getShader(const char* filePath) {
			return GLShaderManager::getShader(filePath);
		};

		inline static void destroy() { GLShaderManager::destroy(); }
	};
#endif
}