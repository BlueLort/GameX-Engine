#pragma once

#include "pch.h"
#include "Config.h"
#include "Maths/GXMaths.h"
#include "Renderer/Enums.h"
namespace gx {
	class GX_DLL GLShader
	{
	public:
		GLShader();
		// activate the shader
		// ------------------------------------------------------------------------
		inline void use() {
			glUseProgram(ID);
		}
		// deactivate shaders
		// ------------------------------------------------------------------------
		inline static void stop() {
			glUseProgram(0);
		}
		// utility uniform functions
		// ------------------------------------------------------------------------
		inline void setBool(const char* name, GXBool value) const {
			GXuint32 loc = glGetUniformLocation(ID, name);
			glUniform1i(loc, static_cast<GXint32>(value));
		}
		// ------------------------------------------------------------------------
		inline void setInt(const char* name, GXint32 value) const{
			GXuint32 loc = glGetUniformLocation(ID, name);
			glUniform1i(loc, value);
		}
		// ------------------------------------------------------------------------

		inline void setUInt(const char* name, GXuint32 value) const {
			GXuint32 loc = glGetUniformLocation(ID, name);
			glUniform1ui(loc, value);
		}
		// ------------------------------------------------------------------------
		inline void setFloat(const  char* name, GXFloat value) const {
			GXuint32 loc = glGetUniformLocation(ID, name);
			glUniform1f(loc, value);
		}
		// ------------------------------------------------------------------------
		inline void setVec2(const  char* name, const GXVec2& value) const {
			GXuint32 loc = glGetUniformLocation(ID, name);
			glUniform2fv(loc, 1, &value[0]);
		}
		inline void setVec2(const char* name, float x, float y) const {
			GXuint32 loc = glGetUniformLocation(ID, name);
			glUniform2f(loc, x, y);
		}
		// ------------------------------------------------------------------------
		inline void setVec3(const  char* name, const GXVec3& value) const {
			GXuint32 loc = glGetUniformLocation(ID, name); 
			glUniform3fv(loc, 1, &value[0]);
		}
		inline void setVec3(const char* name, GXFloat x, GXFloat y, GXFloat z) const {
			GXuint32 loc = glGetUniformLocation(ID, name); 
			glUniform3f(loc, x, y, z);
		}
		// ------------------------------------------------------------------------
		inline void setVec4(const  char* name, const GXVec4& value) const { 
			GXuint32 loc = glGetUniformLocation(ID, name); 
			glUniform4fv(loc, 1, &value[0]);
		}
		inline void setVec4(const  char* name, GXFloat x, GXFloat y, GXFloat z, GXFloat w) const {
			GXuint32 loc = glGetUniformLocation(ID, name);
			glUniform4f(loc, x, y, z, w);
		}
		// ------------------------------------------------------------------------
		inline void setMat2(const char* name, const GXMat2& mat) const {
			GXuint32 loc = glGetUniformLocation(ID, name); 
			glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]);
		}
		// ------------------------------------------------------------------------
		inline void setMat3(const char* name, const GXMat3& mat) const { 
			GXuint32 loc = glGetUniformLocation(ID, name); 
			glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
		}
		// ------------------------------------------------------------------------
		inline void setMat4(const char* name, const GXMat4& mat) const {
			GXuint32 loc = glGetUniformLocation(ID, name);
			glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
		}
		//---------------------------------------------------------------------
		//updates the shader with known Shader Values Such as projection view light pos.
		inline void update(GXMat4 proj, GXMat4 view, GXMat4 model, GXVec3 camPos) {

			//update shader data

		}
		//---------------------------------------------------------------------

		inline void destroy() {
			glDeleteProgram(ID);
		}
		void addShader(GXShaderType shaderType, const char* shaderCode);
	private:
	
		GXuint32 ID;
		GXuint32 compileShader(GXShaderType type, const char* code, char* log);
		void linkProgram(char* log);
	};
}